#include "../../../include/common/util/csv_data_manipulator.hpp"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>

// --- KONSTANTE ---
const char* CSVData::VERSION = "v0.3b";
const char CSVData::CSV_DELIMITER = ',';
const char CSVData::C_STRING_DELIMITER = '"';
const char* CSVData::S_STRING_DELIMITER = "\"";
const char* CSVData::TMP_DELIM_REPLACEMENT = "#|#";
const char CSVData::DECIMAL_DELIMITER = '.';

const int CSVData::ASC = 1;
const int CSVData::DESC = 2;

using namespace std;

// Pomoćna funkcija za sigurno čitanje linija na različitim OS (Windows/Linux)
bool os_safe_getline (istream& is, string& s) {
    s.clear ();
    char c;
    while (is.get (c)) {
        if (c == '\n') return true;
        if (c == '\r') {
            if (is.peek () == '\n') is.get ();
            return true;
        }
        s += c;
    }
    return !s.empty ();
}

static bool is_digits (const std::string& str) {
    if (str.empty ()) return false;

    size_t i = 0;

    if (str[i] == '+' || str[i] == '-') {
        ++i;
        if (i >= str.length ()) return false;
    }

    bool has_digit = false;
    bool has_decimal_point = false;

    for (; i < str.length (); ++i) {
        char c = str[i];

        if (std::isdigit (static_cast<unsigned char> (c))) {
            has_digit = true;
        } else if (c == '.') {
            if (has_decimal_point) return false;
            has_decimal_point = true;
        } else {
            return false;
        }
    }

    return has_digit;
}

// Dodaj i member funkciju
bool CSVData::is_digits (const std::string& str) const {
    return ::is_digits (str);
}

CSVData::CSVData () : m_is_modified (false), m_is_unified (true), m_rows (0), m_cols (0) {}

CSVData::CSVData (const string& filename) : m_is_modified (false), m_is_unified (true), m_rows (0), m_cols (0) {
    read_file (filename);
    m_is_modified = false;
}

// --- GETERI I SETERI ---

const string CSVData::get_value (int row, int col) {
    if (row < 0 || col < 0 || static_cast<size_t> (row) >= m_data.size ()) return "";
    if (static_cast<size_t> (col) >= m_data.at (row).size ()) return "";
    return m_data.at (row).at (col);
}

const vector<string> CSVData::get_row (int row) {
    if (row < 0 || static_cast<size_t> (row) >= m_data.size ()) return vector<string> ();
    return m_data.at (row);
}

void CSVData::set_value (int row, int col, string value) {
    if (row < 0 || col < 0 || static_cast<size_t> (row) >= m_data.size ()) return;
    if (static_cast<size_t> (col) >= m_data.at (row).size ()) return;

    m_data.at (row).at (col) = move (value);
    m_is_modified = true;
}

// --- MANIPULACIJA PODACIMA ---

void CSVData::add_row (vector<string> row_data) {
    size_t new_row_size = row_data.size ();
    if (m_is_unified && !m_data.empty () && new_row_size != static_cast<size_t> (m_cols)) {
        m_is_unified = false;
    }

    m_data.push_back (move (row_data));
    m_rows = static_cast<int> (m_data.size ());
    if (static_cast<size_t> (m_cols) < new_row_size) m_cols = static_cast<int> (new_row_size);
    m_is_modified = true;
}

void CSVData::delete_row (int row) {
    if (row < 0 || static_cast<size_t> (row) >= m_data.size ()) return;
    m_data.erase (m_data.begin () + row);
    m_rows = static_cast<int> (m_data.size ());
    m_is_modified = true;
}

void CSVData::delete_row_if (function<bool (int, const vector<string>&, void*)> cbFun, void* cbData) {
    vector<int> rows_to_delete;
    for (size_t i = 0; i < m_data.size (); ++i) {
        if (cbFun (static_cast<int> (i), m_data.at (i), cbData)) {
            rows_to_delete.push_back (static_cast<int> (i));
        }
    }

    if (!rows_to_delete.empty ()) {
        sort (rows_to_delete.begin (), rows_to_delete.end (), greater<int> ());
        for (int r : rows_to_delete) delete_row (r);
    }
}

void CSVData::delete_col (int col) {
    if (col < 0 || static_cast<size_t> (col) >= static_cast<size_t> (m_cols)) return;
    for (auto& row : m_data) {
        if (static_cast<size_t> (col) < row.size ()) {
            row.erase (row.begin () + col);
        }
    }
    m_is_modified = true;
}

// --- FILER OPERATIONS ---

void CSVData::_read_file (const string& filename, vector<vector<string>>& target, int& cols) {
    ifstream file (filename);
    if (!file.is_open ()) {
        cerr << "Greška: Neuspješno otvaranje fajla " << filename << endl;
        return;
    }

    string line;
    while (os_safe_getline (file, line)) {
        // Logika za handlovanje navodnika (String Delimiter)
        if (count (line.begin (), line.end (), C_STRING_DELIMITER) >= 2) {
            bool in_quotes = false;
            string processed_line;
            for (char c : line) {
                if (c == C_STRING_DELIMITER) in_quotes = !in_quotes;
                if (c == CSV_DELIMITER && in_quotes) {
                    processed_line += TMP_DELIM_REPLACEMENT;
                } else {
                    processed_line += c;
                }
            }
            line = processed_line;
        }

        stringstream ss (line);
        string item;
        vector<string> row;
        while (getline (ss, item, CSV_DELIMITER)) {
            // Vraćanje originalnih zareza unutar stringova
            size_t pos;
            while ((pos = item.find (TMP_DELIM_REPLACEMENT)) != string::npos) {
                item.replace (pos, 3, ",");
            }
            // Uklanjanje navodnika ako postoje
            item.erase (remove (item.begin (), item.end (), C_STRING_DELIMITER), item.end ());
            row.push_back (item);
        }

        if (!row.empty ()) {
            if (cols != 0 && static_cast<size_t> (cols) != row.size ()) m_is_unified = false;
            cols = max (cols, static_cast<int> (row.size ()));
            target.push_back (move (row));
        }
    }
}

void CSVData::read_file (const string& filename) {
    m_data.clear ();
    m_cols = 0;
    _read_file (filename, m_data, m_cols);
    m_rows = static_cast<int> (m_data.size ());
    m_is_modified = false;
}

void CSVData::write_data (const string& filename) {
    ofstream file (filename);
    if (!file.is_open ()) return;

    for (const auto& row : m_data) {
        for (size_t i = 0; i < row.size (); ++i) {
            // Ako podatak sadrži zarez, stavi ga u navodnike
            if (row[i].find (CSV_DELIMITER) != string::npos) {
                file << C_STRING_DELIMITER << row[i] << C_STRING_DELIMITER;
            } else {
                file << row[i];
            }

            if (i < row.size () - 1) file << CSV_DELIMITER;
        }
        file << "\n";
    }
    m_is_modified = false;
}

// --- SORTIRANJE ---

void CSVData::sort_by_col (int col, int order) {
    if (col < 0 || static_cast<size_t> (col) >= static_cast<size_t> (m_cols)) return;

    auto compare = [&] (const vector<string>& a, const vector<string>& b) {
        // Osiguranje da kolona postoji u oba reda (za ne-unifikovane tabele)
        string valA = (static_cast<size_t> (col) < a.size ()) ? a[col] : "";
        string valB = (static_cast<size_t> (col) < b.size ()) ? b[col] : "";

        bool result;
        if (is_digits (valA) && is_digits (valB)) {
            result = stod (valA) < stod (valB);
        } else {
            result = valA < valB;
        }
        return (order == ASC) ? result : !result;
    };

    sort (m_data.begin (), m_data.end (), compare);
    m_is_modified = true;
}

void CSVData::make_data_unique () {
    if (m_data.empty ()) return;
    sort (m_data.begin (), m_data.end ());
    m_data.erase (unique (m_data.begin (), m_data.end ()), m_data.end ());
    m_rows = static_cast<int> (m_data.size ());
    m_is_modified = true;
}