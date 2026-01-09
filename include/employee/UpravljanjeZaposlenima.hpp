#ifndef UPRAVLJANJE_ZAPOSLENIMA_HPP
#define UPRAVLJANJE_ZAPOSLENIMA_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "../common/models/Worker.hpp"

// Funkcije za rad sa CSV fajlovima
std::vector<Worker> readWorkersFromCSV(const std::string& filename);
void writeWorkersToCSV(const std::string& filename, const std::vector<Worker>& workers);

// CRUD operacije
void createNewWorker(std::vector<Worker>& workers, std::string filename);
void viewAllWorkers(const std::vector<Worker>& workers);
void searchWorker(const std::vector<Worker>& workers);
void updateWorker(std::vector<Worker>& workers, std::string filename);
void deleteWorker(std::vector<Worker>& workers, std::string filename);

// Statistika
void viewWorkerStatistics(const std::vector<Worker>& workers);

// Glavna funkcija za upravljanje zaposlenima
void mainUpravljanjeZaposlenima();

#endif // UPRAVLJANJE_ZAPOSLENIMA_HPP