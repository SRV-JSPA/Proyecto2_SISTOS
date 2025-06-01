#include "Scheduler/RoundRobinScheduler.h"
#include <wx/log.h>

RoundRobinScheduler::RoundRobinScheduler(int quantum)
    : m_quantum(quantum), m_timeInQuantum(0), m_justPreempted(false) {}

void RoundRobinScheduler::Initialize() {
    BaseScheduler::Initialize();
    
    m_timeInQuantum = 0;
    m_justPreempted = false;
    
    std::queue<std::shared_ptr<Process>> empty;
    std::swap(m_readyQueue, empty);
    
    wxLogMessage("=== INICIALIZANDO ROUND ROBIN ===");
    wxLogMessage("Quantum configurado: %d", m_quantum);
    
    wxLogMessage("Lista de eventos limpiada por BaseScheduler");
    
    for (auto& process : m_processes) {
        if (process->GetArrivalTime() == 0) {
            m_readyQueue.push(process);
            wxLogMessage("Proceso %s agregado a cola inicial (AT=0)", process->GetPID());
        }
    }
    
    m_currentProcess = nullptr;
    
    wxLogMessage("Inicialización completa. Cola ready: %zu procesos", m_readyQueue.size());
    wxLogMessage("Proceso actual: nullptr (se seleccionará en primer ciclo)");
}

std::shared_ptr<Process> RoundRobinScheduler::GetNextProcess() {
    wxLogMessage("\n--- GetNextProcess() Ciclo %d ---", m_currentCycle);
    wxLogMessage("Quantum: %d, Tiempo en quantum: %d", m_quantum, m_timeInQuantum);
    
    bool allCompleted = true;
    for (auto& process : m_processes) {
        if (!process->IsComplete()) {
            allCompleted = false;
            break;
        }
    }
    
    if (allCompleted) {
        wxLogMessage("Todos los procesos completados");
        return nullptr; 
    }

    for (auto& process : m_processes) {
        if (process->GetArrivalTime() == m_currentCycle && 
            !process->IsComplete() && 
            !process->IsActive()) {
            
            bool alreadyInQueue = false;
            std::queue<std::shared_ptr<Process>> tempQueue = m_readyQueue;
            
            while (!tempQueue.empty()) {
                if (tempQueue.front()->GetPID() == process->GetPID()) {
                    alreadyInQueue = true;
                    break;
                }
                tempQueue.pop();
            }
            
            if (!alreadyInQueue) {
                wxLogMessage("Proceso %s llega en ciclo %d - agregado a cola", 
                           process->GetPID(), m_currentCycle);
                m_readyQueue.push(process);
            }
        }
    }
    
    if (m_currentProcess) {
        wxLogMessage("Proceso actual: %s (Restante: %d)", 
                   m_currentProcess->GetPID(), m_currentProcess->GetRemainingTime());
        
        if (m_currentProcess->IsComplete()) {
            wxLogMessage("Proceso actual completado: %s", m_currentProcess->GetPID());
            m_currentProcess = nullptr;
            m_timeInQuantum = 0;
        }
        else if (m_timeInQuantum >= m_quantum) {
            wxLogMessage("Quantum agotado para: %s - enviando al final de cola", 
                       m_currentProcess->GetPID());
            m_readyQueue.push(m_currentProcess);
            m_currentProcess = nullptr;
            m_timeInQuantum = 0;
        }
        else {
            wxLogMessage("Continuando con proceso actual: %s", m_currentProcess->GetPID());
            return m_currentProcess;
        }
    }
    
    if (!m_currentProcess && !m_readyQueue.empty()) {
        m_currentProcess = m_readyQueue.front();
        m_readyQueue.pop();
        m_timeInQuantum = 0;
        wxLogMessage("Seleccionando nuevo proceso de cola: %s", m_currentProcess->GetPID());
        wxLogMessage("Cola restante: %zu procesos", m_readyQueue.size());
    } else if (!m_currentProcess) {
        wxLogMessage("Cola vacía, no hay proceso para ejecutar");
    }
    
    return m_currentProcess;
}

void RoundRobinScheduler::ExecuteCycle() {
    wxLogMessage("\n=== EJECUTANDO CICLO %d ===", m_currentCycle);
    wxLogMessage("Quantum actual: %d, Tiempo en quantum: %d", m_quantum, m_timeInQuantum);
    
    if (m_currentProcess) {
        wxLogMessage("Proceso actual: %s, Tiempo restante: %d", 
                   m_currentProcess->GetPID(), m_currentProcess->GetRemainingTime());
    } else {
        wxLogMessage("No hay proceso actual");
    }
    wxLogMessage("Tamaño de cola ready: %zu", m_readyQueue.size());

    for (auto& process : m_processes) {
        if (process->GetArrivalTime() <= m_currentCycle && 
            !process->IsActive() && 
            !process->IsComplete()) {
            process->IncrementWaitingTime();
        }
    }

    std::shared_ptr<Process> nextProcess = GetNextProcess();
    
    if (nextProcess) {
        wxLogMessage("Siguiente proceso seleccionado: %s", nextProcess->GetPID());
    } else {
        wxLogMessage("No hay siguiente proceso");
    }

    bool processChanged = (nextProcess != m_currentProcess);
    std::shared_ptr<Process> previousProcess = m_currentProcess;

    if (processChanged) {
        if (previousProcess) {
            previousProcess->Deactivate();
            
            wxLogMessage("Finalizando evento del proceso: %s", previousProcess->GetPID());
            
            if (!m_events.empty() && m_events.back().pid == previousProcess->GetPID()) {
                m_events.back().duration = m_currentCycle - m_events.back().startCycle;
                wxLogMessage("Evento finalizado - Inicio: %d, Duración: %d", 
                           m_events.back().startCycle, m_events.back().duration);
            }
        }
        
        m_currentProcess = nextProcess;
        
        if (m_currentProcess) {
            m_currentProcess->Activate();  
            
            wxLogMessage("Iniciando nuevo evento para: %s en ciclo %d", 
                       m_currentProcess->GetPID(), m_currentCycle);
            
            m_events.emplace_back(m_currentProcess->GetPID(), m_currentCycle, 1);
            wxLogMessage("EVENTO CREADO: %s en ciclo %d", 
                       m_currentProcess->GetPID(), m_currentCycle);
        }
    } 
    else if (m_currentProcess) {
        if (!m_currentProcess->IsActive()) {
            m_currentProcess->Activate();  
            wxLogMessage("Activando proceso continuado: %s", m_currentProcess->GetPID());
        }
        
        if (!m_events.empty() && m_events.back().pid == m_currentProcess->GetPID()) {
            m_events.back().duration = m_currentCycle - m_events.back().startCycle + 1;
            wxLogMessage("Extendiendo evento de %s - Nueva duración: %d", 
                       m_currentProcess->GetPID(), m_events.back().duration);
        } else {
            wxLogMessage("No hay evento previo para %s, creando nuevo evento", 
                       m_currentProcess->GetPID());
            m_events.emplace_back(m_currentProcess->GetPID(), m_currentCycle, 1);
        }
    }
    
    if (m_currentProcess) {
        wxLogMessage("Ejecutando %s - Activo: %s, Tiempo restante ANTES: %d", 
                   m_currentProcess->GetPID(), 
                   m_currentProcess->IsActive() ? "SÍ" : "NO",
                   m_currentProcess->GetRemainingTime());
        
        m_currentProcess->Execute(1);
        m_timeInQuantum++;
        
        wxLogMessage("Ejecutando %s - Tiempo en quantum: %d, Tiempo restante DESPUÉS: %d", 
                   m_currentProcess->GetPID(), m_timeInQuantum, 
                   m_currentProcess->GetRemainingTime());
    } else {
        wxLogMessage("No hay proceso para ejecutar en este ciclo");
    }
    
    bool allProcessesComplete = true;
    for (const auto& process : m_processes) {
        if (!process->IsComplete()) {
            allProcessesComplete = false;
            break;
        }
    }
    
    if (allProcessesComplete && m_readyQueue.empty()) {
        wxLogMessage("*** SIMULACIÓN COMPLETADA EN CICLO %d ***", m_currentCycle);
        wxLogMessage("Todos los procesos terminados y cola vacía");
        m_currentCycle++; 
        return; 
    }
    
    m_currentCycle++;
    
    wxLogMessage("Eventos generados hasta ahora (%zu):", m_events.size());
    for (size_t i = 0; i < m_events.size(); i++) {
        const auto& event = m_events[i];
        wxLogMessage("  [%zu] %s: Ciclos %d-%d (Duración: %d)", 
                   i, event.pid, event.startCycle, 
                   event.startCycle + event.duration - 1, event.duration);
    }
    wxLogMessage("=========================");
}

bool RoundRobinScheduler::IsComplete() const {
    bool allProcessesComplete = true;
    for (const auto& process : m_processes) {
        if (!process->IsComplete()) {
            allProcessesComplete = false;
            break;
        }
    }
    
    bool complete = allProcessesComplete && m_readyQueue.empty() && !m_currentProcess;
    
    if (complete) {
        wxLogMessage("*** SIMULACIÓN ROUND ROBIN COMPLETADA ***");
    }
    
    return complete;
}

void RoundRobinScheduler::SetQuantum(int quantum) {
    m_quantum = quantum;
    wxLogMessage("Quantum actualizado a: %d", quantum);
}

int RoundRobinScheduler::GetQuantum() const {
    return m_quantum;
}