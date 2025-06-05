#include "ProcessMonitor.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <stdexcept>

using namespace std;

constexpr unsigned int DEFAULT_CHECK_INTERVAL = 2000;

ProcessMonitor::ProcessMonitor() 
    : m_isRunning(false), 
      m_checkInterval(DEFAULT_CHECK_INTERVAL),
      m_maxCpuUsage(90.0),
      m_maxMemoryUsage(80.0)
{
    initializeSystemResources();
}

ProcessMonitor::~ProcessMonitor()
{
    if (m_isRunning) {
        stopMonitoring();
    }
    cleanupSystemResources();
}

bool ProcessMonitor::startMonitoring()
{
    if (m_isRunning) {
        cerr << "La surveillance est déjà en cours." << endl;
        return false;
    }

    m_isRunning = true;
    m_monitorThread = thread(&ProcessMonitor::monitorLoop, this);
    
    cout << "Début de la surveillance des processus..." << endl;
    return true;
}

void ProcessMonitor::stopMonitoring()
{
    if (!m_isRunning) {
        return;
    }

    m_isRunning = false;
    if (m_monitorThread.joinable()) {
        m_monitorThread.join();
    }

    cout << "Surveillance des processus arrêtée." << endl;
}

void ProcessMonitor::setCheckInterval(unsigned int milliseconds)
{
    if (milliseconds < 100) {
        throw invalid_argument("L'intervalle doit être d'au moins 100ms");
    }
    m_checkInterval = milliseconds;
}

void ProcessMonitor::setResourceLimits(double maxCpu, double maxMemory)
{
    if (maxCpu <= 0 || maxCpu > 100 || maxMemory <= 0 || maxMemory > 100) {
        throw invalid_argument("Les limites doivent être entre 0 et 100");
    }
    
    m_maxCpuUsage = maxCpu;
    m_maxMemoryUsage = maxMemory;
    
    cout << "Nouvelles limites définies - CPU: " << maxCpu 
              << "%, Mémoire: " << maxMemory << "%" << endl;
}

void ProcessMonitor::monitorLoop()
{
    while (m_isRunning) {
        vector<ProcessInfo> processes = getRunningProcesses();
        
        for (const auto& process : processes) {
            checkProcessResources(process);
        }
        
        this_thread::sleep_for(chrono::milliseconds(m_checkInterval));
    }
}

void ProcessMonitor::checkProcessResources(const ProcessInfo& process)
{
    if (process.cpuUsage > m_maxCpuUsage) {
        handleHighCpuUsage(process);
    }
    
    if (process.memoryUsage > m_maxMemoryUsage) {
        handleHighMemoryUsage(process);
    }
}

void ProcessMonitor::handleHighCpuUsage(const ProcessInfo& process)
{
    cerr << "Alerte! Processus " << process.name 
              << " (PID: " << process.pid << ") utilise " 
              << process.cpuUsage << "% de CPU (limite: " 
              << m_maxCpuUsage << "%)" << endl;
}

void ProcessMonitor::handleHighMemoryUsage(const ProcessInfo& process)
{
    cerr << "Alerte! Processus " << process.name 
              << " (PID: " << process.pid << ") utilise " 
              << process.memoryUsage << "% de mémoire (limite: " 
              << m_maxMemoryUsage << "%)" << endl;
}

void ProcessMonitor::initializeSystemResources()
{
}

void ProcessMonitor::cleanupSystemResources()
{
}

vector<ProcessInfo> ProcessMonitor::getRunningProcesses()
{
    vector<ProcessInfo> processes;
    
    ProcessInfo p1;
    p1.pid = 1234;
    p1.name = "example_process";
    p1.cpuUsage = 25.5;
    p1.memoryUsage = 12.3;
    
    processes.push_back(p1);
    
    return processes;
}