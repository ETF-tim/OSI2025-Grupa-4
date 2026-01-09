#include "../../include/employee/UpravljanjeZaposlenima.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <windows.h>

std::vector<Worker> readWorkersFromCSV(const std::string& filename) {
    std::vector<Worker> workers;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Greska pri otvaranju fajla: " << filename << std::endl;
        return workers;
    }
    
    std::string line;
    if (std::getline(file, line)) {}
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, firstName, lastName, email, phone, roleStr, pinStr;
        
        std::getline(ss, idStr, ',');
        std::getline(ss, firstName, ',');
        std::getline(ss, lastName, ',');
        std::getline(ss, email, ',');
        std::getline(ss, phone, ',');
        std::getline(ss, pinStr, ',');
        std::getline(ss, roleStr);
        try {
            int id = std::stoi(idStr);
            int pin = std::stoi(pinStr);
            
            Role role;
            if (roleStr == "RECEPTIONIST") role = Role::RECEPTIONIST;
            else if (roleStr == "TECHNICIAN") role = Role::TECHNICIAN;
            else if (roleStr == "SERVICE_OWNER") role = Role::SERVICE_OWNER;
            else role = Role::RECEPTIONIST;
            
            Worker w(id, firstName, lastName, email, phone, pin, role);
            workers.push_back(w);
        } catch (const std::exception& e) {
            std::cerr << "Greska pri parsiranju linije: " << line << std::endl;
        }
    }
    
    file.close();
    return workers;
}

void writeWorkersToCSV(const std::string& filename, const std::vector<Worker>& workers) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Greska pri otvaranju fajla za pisanje: " << filename << std::endl;
        return;
    }
    
    file << "ID,FirstName,LastName,Email,Phone,Role,PIN\n";
    
    for (const auto& worker : workers) {
        std::string roleStr;
        switch (worker.getRole()) {
            case Role::RECEPTIONIST: roleStr = "RECEPTIONIST"; break;
            case Role::TECHNICIAN: roleStr = "TECHNICIAN"; break;
            case Role::SERVICE_OWNER: roleStr = "SERVICE_OWNER"; break;
            default: roleStr = "RECEPTIONIST";
        }
        
        file << worker.getId() << ","
             << worker.getFirstName() << ","
             << worker.getLastName() << ","
             << worker.getEmail() << ","
             << worker.getPhone() << ","
             << roleStr << ","
             << worker.getPin() << "\n";
    }
    
    file.close();
}

void createNewWorker(std::vector<Worker>& workers, std::string filename) {
    std::cout << "\n=== KREIRANJE NOVOG ZAPOSLENOG ===\n";
    
    int maxId = 0;
    for (const auto& worker : workers) {
        if (worker.getId() > maxId) {
            maxId = worker.getId();
        }
    }
    
    int newId = maxId + 1;
    std::string firstName, lastName, email,phone;
    int pin, roleChoice;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Unesite ime: ";
    std::getline(std::cin, firstName);
    
    std::cout << "Unesite prezime: ";
    std::getline(std::cin, lastName);
    
    std::cout << "Unesite email: ";
    std::getline(std::cin, email);
    
    std::cout << "Unesite broj telefona: ";
    std::getline(std::cin, phone);
    
    std::cout << "Unesite PIN (4 cifre): ";
    std::cin >> pin;
    
    std::cout << "\nOdaberite ulogu:\n";
    std::cout << "1. RECEPTIONIST (recepcioner)\n";
    std::cout << "2. TECHNICIAN (tehnicar)\n";
    std::cout << "3. SERVICE_OWNER (vlasnik servisa)\n";
    std::cout << "Unesite izbor (1-3): ";
    std::cin >> roleChoice;
    
    Role role;
    switch (roleChoice) {
        case 1: role = Role::RECEPTIONIST; break;
        case 2: role = Role::TECHNICIAN; break;
        case 3: role = Role::SERVICE_OWNER; break;
        default: 
            std::cout << "Pogresan izbor, postavljam RECEPTIONIST.\n";
            role = Role::RECEPTIONIST;
    }
    
    Worker newWorker(newId, firstName, lastName, email, phone, pin, role);
    workers.push_back(newWorker);
    writeWorkersToCSV(filename, workers);
    
    std::cout << "\n=====================================\n";
    std::cout << "ZAPOSLENI USPJESNO KREIRAN!\n";
    std::cout << "=====================================\n";
    std::cout << "ID: " << newId << "\n";
    std::cout << "Ime i prezime: " << firstName << " " << lastName << "\n";
    std::cout << "Email: " << email << "\n";
    std::cout << "Telefon: " << phone << "\n";
    std::cout << "PIN: " << pin << "\n";
    std::cout << "Uloga: ";
    switch (role) {
        case Role::RECEPTIONIST: std::cout << "RECEPTIONIST"; break;
        case Role::TECHNICIAN: std::cout << "TECHNICIAN"; break;
        case Role::SERVICE_OWNER: std::cout << "SERVICE_OWNER"; break;
    }
    std::cout << "\n=====================================\n";
}

void viewAllWorkers(const std::vector<Worker>& workers) {
    std::cout << "\n=== POPIS SVIH ZAPOSLENIH ===\n";
    
    if (workers.empty()) {
        std::cout << "Nema zaposlenih u bazi.\n";
        return;
    }
    
    std::cout << std::left 
              << std::setw(5) << "ID" 
              << std::setw(15) << "IME" 
              << std::setw(15) << "PREZIME" 
              << std::setw(18) << "ULOGA" 
              << std::endl;
    std::cout << std::string(103, '=') << std::endl;
    
    for (const auto& worker : workers) {
        std::string roleStr;
        switch (worker.getRole()) {
            case Role::RECEPTIONIST: roleStr = "RECEPTIONIST"; break;
            case Role::TECHNICIAN: roleStr = "TECHNICIAN"; break;
            case Role::SERVICE_OWNER: roleStr = "SERVICE_OWNER"; break;
        }
        
        std::cout << std::left 
                  << std::setw(5) << worker.getId() 
                  << std::setw(15) << worker.getFirstName() 
                  << std::setw(15) << worker.getLastName() 
                  << std::setw(18) << roleStr 
                  << std::endl;
    }
    
    std::cout << "\nUkupno zaposlenih: " << workers.size() << std::endl;
}

void searchWorker(const std::vector<Worker>& workers) {
    viewAllWorkers(workers);
    std::cout << "\n=== PRETRAGA ZAPOSLENIH ===\n";
    
    if (workers.empty()) {
        std::cout << "Nema zaposlenih u bazi.\n";
        return;
    }
    
    int searchChoice;
    std::cout << "Pretraga po:\n";
    std::cout << "1. ID-u\n";
    std::cout << "2. Prezimenu\n";
    std::cout << "3. Ulozi\n";
    std::cout << "Unesite izbor (1-3): ";
    std::cin >> searchChoice;
    
    std::vector<Worker> results;
    
    switch (searchChoice) {
        case 1: {
            int id;
            std::cout << "Unesite ID za pretragu: ";
            std::cin >> id;
            
            for (const auto& worker : workers) {
                if (worker.getId() == id) {
                    results.push_back(worker);
                }
            }
            break;
        }
        case 2: {
            std::string lastName;
            std::cin.ignore();
            std::cout << "Unesite prezime za pretragu: ";
            std::getline(std::cin, lastName);
            
            for (const auto& worker : workers) {
                if (worker.getLastName().find(lastName) != std::string::npos) {
                    results.push_back(worker);
                }
            }
            break;
        }
        case 3: {
            int roleChoice;
            std::cout << "\nOdaberite ulogu za pretragu:\n";
            std::cout << "1. RECEPTIONIST\n";
            std::cout << "2. TECHNICIAN\n";
            std::cout << "3. SERVICE_OWNER\n";
            std::cout << "Unesite izbor (1-3): ";
            std::cin >> roleChoice;
            
            Role searchRole;
            switch (roleChoice) {
                case 1: searchRole = Role::RECEPTIONIST; break;
                case 2: searchRole = Role::TECHNICIAN; break;
                case 3: searchRole = Role::SERVICE_OWNER; break;
                default: searchRole = Role::RECEPTIONIST;
            }
            
            for (const auto& worker : workers) {
                if (worker.getRole() == searchRole) {
                    results.push_back(worker);
                }
            }
            break;
        }
        default:
            std::cout << "Pogresan izbor.\n";
            return;
    }
    
    if (results.empty()) {
        std::cout << "Nema rezultata pretrage.\n";
    } else {
        std::cout << "\n=== REZULTATI PRETRAGE ===\n";
        std::cout << "Pronadjeno zaposlenih: " << results.size() << "\n\n";
        
        std::cout << std::left 
                  << std::setw(5) << "ID" 
                  << std::setw(15) << "IME" 
                  << std::setw(15) << "PREZIME" 
                  << std::setw(35) << "EMAIL" 
                  << std::setw(12) << "TELEFON" 
                  << std::setw(18) << "ULOGA" 
                  << std::setw(10) << "PIN" 
                  << std::endl;
        std::cout << std::string(103, '-') << std::endl;
        
        for (const auto& worker : results) {
            std::string roleStr;
            switch (worker.getRole()) {
                case Role::RECEPTIONIST: roleStr = "RECEPTIONIST"; break;
                case Role::TECHNICIAN: roleStr = "TECHNICIAN"; break;
                case Role::SERVICE_OWNER: roleStr = "SERVICE_OWNER"; break;
            }
            
            std::cout << std::left 
                      << std::setw(5) << worker.getId() 
                      << std::setw(15) << worker.getFirstName() 
                      << std::setw(15) << worker.getLastName() 
                      << std::setw(35) << worker.getEmail() 
                      << std::setw(12) << worker.getPhone() 
                      << std::setw(18) << roleStr 
                      << std::setw(10) << worker.getPin() 
                      << std::endl;
        }
    }
}

void updateWorker(std::vector<Worker>& workers, std::string filename) {
    std::cout << "\n=== AZURIRANJE ZAPOSLENOG ===\n";
    
    if (workers.empty()) {
        std::cout << "Nema zaposlenih za azuriranje.\n";
        return;
    }
    
    viewAllWorkers(workers);
    
    int id;
    std::cout << "\nUnesite ID zaposlenog za azuriranje: ";
    std::cin >> id;
    
    auto it = std::find_if(workers.begin(), workers.end(),
        [id](const Worker& w) { return w.getId() == id; });
    
    if (it == workers.end()) {
        std::cout << "Zaposleni sa ID " << id << " ne postoji.\n";
        return;
    }
    
    Worker& worker = *it;
    
    std::cout << "\nTrenutni podaci o zaposlenom:\n";
    std::cout << "===============================\n";
    std::cout << "1. Ime: " << worker.getFirstName() << "\n";
    std::cout << "2. Prezime: " << worker.getLastName() << "\n";
    std::cout << "3. Email: " << worker.getEmail() << "\n";
    std::cout << "4. Telefon: " << worker.getPhone() << "\n";
    std::cout << "5. PIN: " << worker.getPin() << "\n";
    std::cout << "6. Uloga: ";
    switch (worker.getRole()) {
        case Role::RECEPTIONIST: std::cout << "RECEPTIONIST"; break;
        case Role::TECHNICIAN: std::cout << "TECHNICIAN"; break;
        case Role::SERVICE_OWNER: std::cout << "SERVICE_OWNER"; break;
    }
    std::cout << "\n===============================\n";
    
    int fieldChoice;
    std::cout << "\nKoje polje zelite azurirati?\n";
    std::cout << "1. Ime\n";
    std::cout << "2. Prezime\n";
    std::cout << "3. Email\n";
    std::cout << "4. Telefon\n";
    std::cout << "5. PIN\n";
    std::cout << "6. Uloga\n";
    std::cout << "0. Nazad\n";
    std::cout << "Unesite izbor: ";
    std::cin >> fieldChoice;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (fieldChoice) {
        case 1: {
            std::string newFirstName;
            std::cout << "Unesite novo ime: ";
            std::getline(std::cin, newFirstName);
            worker.setFirstName(newFirstName);
            std::cout << "Ime azurirano.\n";
            break;
        }
        case 2: {
            std::string newLastName;
            std::cout << "Unesite novo prezime: ";
            std::getline(std::cin, newLastName);
            worker.setLastName(newLastName);
            std::cout << "Prezime azurirano.\n";
            break;
        }
        case 3: {
            std::string newEmail;
            std::cout << "Unesite novi email: ";
            std::getline(std::cin, newEmail);
            worker.setEmail(newEmail);
            std::cout << "Email azuriran.\n";
            break;
        }
        case 4: {
            std::string newPhone;
            std::cout << "Unesite novi broj telefona: ";
            std::getline(std::cin, newPhone);
            worker.setPhone(newPhone);
            std::cout << "Telefon azuriran.\n";
            break;
        }
        case 5: {
            int newPin;
            std::cout << "Unesite novi PIN: ";
            std::cin >> newPin;
            worker.setPin(newPin);
            std::cout << "PIN azuriran.\n";
            break;
        }
        case 6: {
            int roleChoice;
            std::cout << "\nOdaberite novu ulogu:\n";
            std::cout << "1. RECEPTIONIST\n";
            std::cout << "2. TECHNICIAN\n";
            std::cout << "3. SERVICE_OWNER\n";
            std::cout << "Unesite izbor (1-3): ";
            std::cin >> roleChoice;
            
            Role newRole;
            switch (roleChoice) {
                case 1: newRole = Role::RECEPTIONIST; break;
                case 2: newRole = Role::TECHNICIAN; break;
                case 3: newRole = Role::SERVICE_OWNER; break;
                default: 
                    std::cout << "Pogresan izbor, uloga nije promijenjena.\n";
                    return;
            }
            worker.setRole(newRole);
            std::cout << "Uloga azurirana.\n";
            break;
        }
        case 0:
            std::cout << "Azuriranje otkazano.\n";
            return;
        default:
            std::cout << "Pogresan izbor.\n";
            return;
    }
    
    writeWorkersToCSV(filename, workers);
    std::cout << "\nPodaci o zaposlenom uspjesno azurirani!\n";
}

void deleteWorker(std::vector<Worker>& workers, std::string filename) {
    std::cout << "\n=== BRISANJE ZAPOSLENOG ===\n";
    
    if (workers.empty()) {
        std::cout << "Nema zaposlenih za brisanje.\n";
        return;
    }
    
    viewAllWorkers(workers);
    
    int id;
    std::cout << "\nUnesite ID zaposlenog za brisanje: ";
    std::cin >> id;
    
    auto it = std::find_if(workers.begin(), workers.end(),
        [id](const Worker& w) { return w.getId() == id; });
    
    if (it == workers.end()) {
        std::cout << "Zaposleni sa ID " << id << " ne postoji.\n";
        return;
    }
    
    std::cout << "\nZAPOSLENI ZA BRISANJE:\n";
    std::cout << "======================\n";
    std::cout << "ID: " << it->getId() << "\n";
    std::cout << "Ime i prezime: " << it->getFirstName() << " " << it->getLastName() << "\n";
    std::cout << "Email: " << it->getEmail() << "\n";
    std::cout << "Telefon: " << it->getPhone() << "\n";
    std::cout << "PIN: " << it->getPin() << "\n";
    std::cout << "Uloga: ";
    switch (it->getRole()) {
        case Role::RECEPTIONIST: std::cout << "RECEPTIONIST"; break;
        case Role::TECHNICIAN: std::cout << "TECHNICIAN"; break;
        case Role::SERVICE_OWNER: std::cout << "SERVICE_OWNER"; break;
    }
    std::cout << "\n======================\n";
    
    char confirm;
    std::cout << "\nDa li ste SIGURNI da zelite obrisati ovog zaposlenog? (d/n): ";
    std::cin >> confirm;
    
    if (confirm == 'd' || confirm == 'D') {
        workers.erase(it);
        writeWorkersToCSV(filename, workers);
        std::cout << "\nZaposleni uspjesno obrisan!\n";
    } else {
        std::cout << "\nBrisanje otkazano.\n";
    }
}

void viewWorkerStatistics(const std::vector<Worker>& workers) {
    std::cout << "\n=== STATISTIKA ZAPOSLENIH ===\n";
    
    if (workers.empty()) {
        std::cout << "Nema zaposlenih u bazi.\n";
        return;
    }
    
    int receptionists = 0;
    int technicians = 0;
    int serviceOwners = 0;
    
    for (const auto& worker : workers) {
        switch (worker.getRole()) {
            case Role::RECEPTIONIST: receptionists++; break;
            case Role::TECHNICIAN: technicians++; break;
            case Role::SERVICE_OWNER: serviceOwners++; break;
        }
    }
    
    std::cout << "Ukupno zaposlenih: " << workers.size() << "\n";
    std::cout << "--------------------------------\n";
    std::cout << "RECEPTIONIST: " << receptionists << " (" 
              << std::fixed << std::setprecision(1) 
              << (workers.size() > 0 ? (receptionists * 100.0 / workers.size()) : 0) << "%)\n";
    std::cout << "TECHNICIAN: " << technicians << " (" 
              << (workers.size() > 0 ? (technicians * 100.0 / workers.size()) : 0) << "%)\n";
    std::cout << "SERVICE_OWNER: " << serviceOwners << " (" 
              << (workers.size() > 0 ? (serviceOwners * 100.0 / workers.size()) : 0) << "%)\n";
    std::cout << "--------------------------------\n";
    
    // Najmanji i najveći ID
    if (!workers.empty()) {
        int minId = workers[0].getId();
        int maxId = workers[0].getId();
        for (const auto& worker : workers) {
            if (worker.getId() < minId) minId = worker.getId();
            if (worker.getId() > maxId) maxId = worker.getId();
        }
        std::cout << "Raspon ID-eva: " << minId << " - " << maxId << "\n";
    }
}

void mainUpravljanjeZaposlenima() {

    std::string lokacija = "data/employee.csv";
    std::vector<Worker> workers = readWorkersFromCSV(lokacija);
    
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "     SISTEM ZA UPRAVLJANJE ZAPOSLENIMA    \n";
        std::cout << "========================================\n";
        std::cout << "Trenutno zaposlenih: " << workers.size() << "\n";
        std::cout << "----------------------------------------\n";
        std::cout << "1. Pregled svih zaposlenih\n";
        std::cout << "2. Pretraga zaposlenih\n";
        std::cout << "3. Kreiranje novog zaposlenog\n";
        std::cout << "4. Azuriranje podataka o zaposlenom\n";
        std::cout << "5. Brisanje zaposlenog\n";
        std::cout << "6. Statistika zaposlenih\n";
        std::cout << "0. Izlaz iz programa\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Unesite izbor: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                viewAllWorkers(workers);
                break;
                
            case 2:
                searchWorker(workers);
                break;
                
            case 3:
                createNewWorker(workers, lokacija);
                break;
                
            case 4:
                updateWorker(workers, lokacija);
                break;
                
            case 5:
                deleteWorker(workers, lokacija);
                break;
                
            case 6:
                viewWorkerStatistics(workers);
                break;
                
            case 0:
                std::cout << "\n========================================\n";
                std::cout << "     Hvala sto koristite nas sistem!    \n";
                std::cout << "========================================\n";
                break;
                
            default:
                std::cout << "Pogresan izbor. Pokusajte ponovo.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    }
}