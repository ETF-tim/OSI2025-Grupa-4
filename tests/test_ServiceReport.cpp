#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>

#include "../include/common/models/ServiceReport.hpp"
#include "../include/common/util/Validate.hpp"

TEST (ServiceReportTest, WorkOrderID_GetSet) {
    ServiceReport sr;
    sr.setWorkOrderID (123);
    EXPECT_EQ (sr.getWorkOrderID (), 123);
}

TEST (ServiceReportTest, WorkOrderID_Validation) {
    ServiceReport sr;
    EXPECT_THROW (sr.setWorkOrderID (-1), std::invalid_argument);
    EXPECT_THROW (sr.setWorkOrderID (std::numeric_limits<int>::max ()), std::invalid_argument);
}

TEST (ServiceReportTest, Id_GetSet) {
    ServiceReport sr;
    sr.setId (7);
    EXPECT_EQ (sr.getId (), 7);
}

TEST (ServiceReportTest, Id_Validation) {
    ServiceReport sr;
    EXPECT_THROW (sr.setId (-10), std::invalid_argument);
    EXPECT_THROW (sr.setId (std::numeric_limits<int>::max ()), std::invalid_argument);
}
