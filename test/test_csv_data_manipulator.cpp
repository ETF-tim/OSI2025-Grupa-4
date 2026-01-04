#include <iostream>
#include <vector>
#include <string>
#include "../include/util/csv_data_manipulator.hpp"

using namespace std;

void print_csv_data(CSVData &csv)
{
    cout << "\n=== CSV Data (" << csv.rows() << " rows, " << csv.columns() << " cols) ===" << endl;
    for (int i = 0; i < csv.rows(); i++)
    {
        vector<string> row = csv.get_row(i);
        for (int j = 0; j < row.size(); j++)
        {
            cout << row[j];
            if (j < row.size() - 1)
                cout << " | ";
        }
        cout << endl;
    }
    cout << "================================\n"
         << endl;
}

void test_basic_operations()
{
    cout << "\n### Test 1: Basic Operations ###\n"
         << endl;

    CSVData csv;

    // Add rows
    vector<string> row1 = {"ID", "Name", "Age", "Salary"};
    vector<string> row2 = {"1", "John", "30", "50000.50"};
    vector<string> row3 = {"2", "Jane", "25", "60000.75"};
    vector<string> row4 = {"3", "Bob", "35", "55000.00"};

    csv.add_row(row1);
    csv.add_row(row2);
    csv.add_row(row3);
    csv.add_row(row4);

    cout << "After adding rows:" << endl;
    print_csv_data(csv);

    // Get specific value
    cout << "Value at (1, 1): " << csv.get_value(1, 1) << endl;

    // Set value
    csv.set_value(1, 1, "Johnny");
    cout << "\nAfter setting value at (1, 1) to 'Johnny':" << endl;
    print_csv_data(csv);
}

void test_sorting()
{
    cout << "\n### Test 2: Sorting ###\n"
         << endl;

    CSVData csv;

    vector<string> header = {"ID", "Name", "Score"};
    vector<string> row1 = {"3", "Charlie", "85"};
    vector<string> row2 = {"1", "Alice", "95"};
    vector<string> row3 = {"2", "Bob", "75"};

    csv.add_row(header);
    csv.add_row(row1);
    csv.add_row(row2);
    csv.add_row(row3);

    cout << "Original data:" << endl;
    print_csv_data(csv);

    // Sort by ID (column 0) ascending
    csv.sort_by_col(0, CSVData::ASC);
    cout << "After sorting by ID (ASC):" << endl;
    print_csv_data(csv);

    // Sort by Name (column 1) descending
    csv.sort_by_col(1, CSVData::DESC);
    cout << "After sorting by Name (DESC):" << endl;
    print_csv_data(csv);

    // Sort by Score (column 2) ascending
    csv.sort_by_col(2, CSVData::ASC);
    cout << "After sorting by Score (ASC):" << endl;
    print_csv_data(csv);
}

void test_delete_operations()
{
    cout << "\n### Test 3: Delete Operations ###\n"
         << endl;

    CSVData csv;

    csv.add_row({"ID", "Name", "Status"});
    csv.add_row({"1", "John", "Active"});
    csv.add_row({"2", "Jane", "Inactive"});
    csv.add_row({"3", "Bob", "Active"});
    csv.add_row({"4", "Alice", "Inactive"});

    cout << "Original data:" << endl;
    print_csv_data(csv);

    // Delete row 2
    csv.delete_row(2);
    cout << "After deleting row 2:" << endl;
    print_csv_data(csv);

    // Delete column 1
    csv.delete_col(1);
    cout << "After deleting column 1:" << endl;
    print_csv_data(csv);
}

void test_conditional_delete()
{
    cout << "\n### Test 4: Conditional Delete ###\n"
         << endl;

    CSVData csv;

    csv.add_row({"ID", "Name", "Age"});
    csv.add_row({"1", "John", "30"});
    csv.add_row({"2", "Jane", "25"});
    csv.add_row({"3", "Bob", "40"});
    csv.add_row({"4", "Alice", "22"});

    cout << "Original data:" << endl;
    print_csv_data(csv);

    // Delete rows where age > 30
    csv.delete_row_if([](int row, int col, const string &val)
                      {
        if (col == 2 && row > 0) { // Skip header
            try {
                return stoi(val) > 30;
            } catch (...) {
                return false;
            }
        }
        return false; });

    cout << "After deleting rows with age > 30:" << endl;
    print_csv_data(csv);
}

void test_file_operations()
{
    cout << "\n### Test 5: File Operations ###\n"
         << endl;

    CSVData csv;

    csv.add_row({"ID", "Product", "Price"});
    csv.add_row({"1", "Laptop", "999.99"});
    csv.add_row({"2", "Mouse", "25.50"});
    csv.add_row({"3", "Keyboard", "75.00"});

    cout << "Data to write:" << endl;
    print_csv_data(csv);

    // Write to file
    string filename = "test_output.csv";
    csv.write_data(filename);
    cout << "Data written to '" << filename << "'" << endl;

    // Read from file
    CSVData csv2(filename);
    cout << "\nData read from file:" << endl;
    print_csv_data(csv2);
}

void test_date_conversion()
{
    cout << "\n### Test 6: Date Format Conversion ###\n"
         << endl;

    CSVData csv;

    csv.add_row({"ID", "Event", "Date"});
    csv.add_row({"1", "Meeting", "2026-01-15"});
    csv.add_row({"2", "Conference", "2026-02-20"});
    csv.add_row({"3", "Workshop", "2026-03-10"});

    cout << "Original dates (YYYY-MM-DD):" << endl;
    print_csv_data(csv);

    // Convert date format from YYYY-MM-DD to DD/MM/YYYY
    csv.convert_date_format("%Y-%m-%d", "%d/%m/%Y", 2);

    cout << "After converting to DD/MM/YYYY:" << endl;
    print_csv_data(csv);
}

void test_insert_row()
{
    cout << "\n### Test 7: Insert Row at Position ###\n"
         << endl;

    CSVData csv;

    csv.add_row({"ID", "Name"});
    csv.add_row({"1", "First"});
    csv.add_row({"3", "Third"});

    cout << "Before insert:" << endl;
    print_csv_data(csv);

    // Insert at position 2
    vector<string> new_row = {"2", "Second"};
    csv.add_row(new_row, 2);

    cout << "After inserting at position 2:" << endl;
    print_csv_data(csv);
}

int main()
{
    cout << "==================================================" << endl;
    cout << "CSV Data Manipulator Test Suite" << endl;
    cout << "Version: " << CSVData().get_version() << endl;
    cout << "==================================================" << endl;

    try
    {
        test_basic_operations();
        test_sorting();
        test_delete_operations();
        test_conditional_delete();
        test_file_operations();
        test_date_conversion();
        test_insert_row();

        cout << "\n==================================================" << endl;
        cout << "All tests completed successfully!" << endl;
        cout << "==================================================" << endl;
    }
    catch (const exception &e)
    {
        cerr << "\nError occurred: " << e.what() << endl;
        return 1;
    }

    getchar();
    return 0;
}