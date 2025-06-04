std::string ProcessMonitor::getProcessInfo( ){
        for (const auto& process : ap) {
            info += "PID : " + std::to_string(process.PID) + "\n";
            info += "Utilisateur : " + process.user + "\n";
            info += "Chemin de l'exécutable : " + process.pathName + "\n";
            info += "Utilisation du processeur : " + std::to_string(process.cpu) + "\n";
            info += "Utilisation de la mémoire : " + std::to_string(process.memory) + " Mo\n\n";
        }
        return info;
    };