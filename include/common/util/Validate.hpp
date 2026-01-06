#pragma once

#include <ctime>
#include <map>
#include <string>

enum class PaymentMethod;
enum class State;
enum class WorkOrderStatus;
enum class Role;

namespace Validate {
// ---Common---
bool isValidId (int id);
bool isValidNonNegative (int amount);  // (za amount, criticalAmount, price)

// Client - nema

// Bill
bool isValidPaymentMethod (PaymentMethod paymentMethod);
bool isValidPrice (double price);

// Device
bool isValidBrand (const std::string& brand);
bool isValidModel (const std::string& model);
bool isValidIMEI (const std::string& deviceIMEI);
bool isValidState (State state);

// Part
bool isValidPartName (const std::string& name);  // preimenovao iz "isValidName()"

// Person
bool isValidFirstName (const std::string& firstName);
bool isValidLastName (const std::string& lastName);
bool isValidEmail (const std::string& email);
bool isValidPhone (const std::string& phone);
bool isValidPin (int pin);

// Receipt order
bool isValidDescription (const std::string& description);
bool isValidPriceAssessment (double priceAssessment);

// Service report - nema

// Worker
bool isValidRole (Role role);

// Work order
bool isValidStatus (WorkOrderStatus status);
bool isValidStartDate (std::time_t startDate);
bool isValidEndDate (std::time_t startDate, std::time_t endDate);
bool isValidUsedParts (const std::map<int, int>& usedParts);
bool isValidComment (const std::string& comment);
}  // namespace Validate