#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

// Adjust include path to match project layout (same as implementation)
#include "../include/common/util/csv_data_manipulator.hpp"

static int count_rows (csv::CSVData& d) {
    int i = 0;
    while (true) {
        auto r = d.get_row (i);
        if (r.empty ()) break;
        ++i;
    }
    return i;
}

TEST (CSVDataTest, ReadQuotedComma) {
    const char* fname = "tmp_csv_quoted.csv";
    {
        ofstream f (fname, ios::binary);
        f << "a,\"b,bb\",c\n";
        f << "1,2,3\n";
    }

    csv::CSVData d;
    d.read_file (fname);

    EXPECT_EQ (d.get_value (0, 0), "a");
    EXPECT_EQ (d.get_value (0, 1), "b,bb");
    EXPECT_EQ (d.get_value (0, 2), "c");
    EXPECT_EQ (d.get_value (1, 0), "1");
    EXPECT_EQ (d.get_value (1, 1), "2");
    EXPECT_EQ (d.get_value (1, 2), "3");

    std::remove (fname);
}

TEST (CSVDataTest, AddSetDeleteColRow) {
    csv::CSVData d;
    d.add_row (vector<std::string>{"x", "y", "z"});
    EXPECT_EQ (d.get_value (0, 0), "x");
    EXPECT_EQ (d.get_value (0, 1), "y");
    EXPECT_EQ (d.get_value (0, 2), "z");

    d.set_value (0, 1, "Y");
    EXPECT_EQ (d.get_value (0, 1), "Y");

    d.delete_col (0);  // remove "x", so "Y" becomes column 0
    EXPECT_EQ (d.get_value (0, 0), "Y");
    EXPECT_EQ (d.get_value (0, 1), "z");

    d.add_row (vector<std::string>{"keep"});
    EXPECT_EQ (count_rows (d), 2);

    d.delete_row (1);
    EXPECT_EQ (count_rows (d), 1);
}

TEST (CSVDataTest, DeleteRowIfCallback) {
    csv::CSVData d;
    d.add_row (vector<std::string>{"keep", "1"});
    d.add_row (vector<std::string>{"del", "2"});
    d.add_row (vector<std::string>{"del", "3"});
    EXPECT_EQ (count_rows (d), 3);

    auto cb = [] (int idx, const vector<std::string>& row, void* data) -> bool {
        // delete rows where first column == "del"
        return !row.empty () && row[0] == "del";
    };

    d.delete_row_if (cb, nullptr);
    EXPECT_EQ (count_rows (d), 1);
    EXPECT_EQ (d.get_value (0, 0), "keep");
}

TEST (CSVDataTest, SortByColumnNumericAndstd::string) {
    csv::CSVData d;
    d.add_row (vector<std::string>{"2"});
    d.add_row (vector<std::string>{"10"});
    d.add_row (vector<std::string>{"1"});

    // numeric ascending
    d.sort_by_col (0, csv::CSVData::ASC);
    EXPECT_EQ (d.get_value (0, 0), "1");
    EXPECT_EQ (d.get_value (1, 0), "2");
    EXPECT_EQ (d.get_value (2, 0), "10");

    // numeric descending
    d.sort_by_col (0, csv::CSVData::DESC);
    EXPECT_EQ (d.get_value (0, 0), "10");
    EXPECT_EQ (d.get_value (2, 0), "1");

    // std::string sort
    csv::CSVData s;
    s.add_row (vector<std::string>{"b"});
    s.add_row (vector<std::string>{"a"});
    s.add_row (vector<std::string>{"c"});
    s.sort_by_col (0, csv::CSVData::ASC);
    EXPECT_EQ (s.get_value (0, 0), "a");
    EXPECT_EQ (s.get_value (1, 0), "b");
    EXPECT_EQ (s.get_value (2, 0), "c");
}

TEST (CSVDataTest, MakeDataUnique) {
    csv::CSVData d;
    d.add_row (vector<std::string>{"a", "1"});
    d.add_row (vector<std::string>{"b", "2"});
    d.add_row (vector<std::string>{"a", "1"});
    d.add_row (vector<std::string>{"b", "2"});
    d.add_row (vector<std::string>{"c", "3"});

    EXPECT_EQ (count_rows (d), 5);
    d.make_data_unique ();
    // after uniquing should have 3 distinct rows (order unspecified after internal sort)
    EXPECT_EQ (count_rows (d), 3);

    // ensure each expected row exists somewhere
    bool has_a1 = false, has_b2 = false, has_c3 = false;
    for (int i = 0;; ++i) {
        auto r = d.get_row (i);
        if (r.empty ()) break;
        if (r.size () >= 2) {
            if (r[0] == "a" && r[1] == "1") has_a1 = true;
            if (r[0] == "b" && r[1] == "2") has_b2 = true;
            if (r[0] == "c" && r[1] == "3") has_c3 = true;
        }
    }
    EXPECT_TRUE (has_a1 && has_b2 && has_c3);
}

TEST (CSVDataTest, WriteAndReadRoundTripWithQuotedField) {
    const char* fname = "tmp_csv_write.csv";
    {
        csv::CSVData d;
        d.add_row (vector<std::string>{"normal", "a,b,comma"});
        d.add_row (vector<std::string>{"plain", "value"});
        d.write_data (fname);
    }

    csv::CSVData r;
    r.read_file (fname);
    EXPECT_EQ (r.get_value (0, 0), "normal");
    EXPECT_EQ (r.get_value (0, 1), "a,b,comma");
    EXPECT_EQ (r.get_value (1, 0), "plain");
    EXPECT_EQ (r.get_value (1, 1), "value");

    std::remove (fname);
}

int main (int argc, char** argv) {
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS ();
}