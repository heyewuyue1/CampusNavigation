#include "model/model.h"
#include <gtest/gtest.h>
#include "model/logical/courseHub.h"
#include "model/logical/logical_model.h"
#include "model/model_hub.h"
#include "model/physical/building.h"
#include "model/physical/path.h"

using namespace std;
class Env : public ::testing::Test {
   protected:
    virtual void SetUp() {
        auto b1 = std::make_shared<Path>();
        b1->set_id(mh.add(b1));
        auto t = std::static_pointer_cast<Path>(mh.get(0u));
        pid = b1->get_id();
    }
    ModelHub mh;
    Id pid;
};
TEST_F(Env, modelhub_building_test) {
    string building_names[5] = {"学一", "教室食堂一", "教师食堂二", "雁南", "雁北"};
    for (int i = 0; i < 5; i++) {
        auto b1 = std::make_shared<Building>(building_names[i]);
        b1->set_id(mh.add(b1));
    }
    for (uint32_t i = 0; i < 5u; i++) {
        ASSERT_EQ(mh.get(i + 1)->get_name(), building_names[i]);
    }
}
TEST_F(Env, modelhub_path_test) {
    auto b1 = std::make_shared<Path>();
    b1->set_id(mh.add(b1));
}
TEST_F(Env, connection_test) {
    string building_names[5] = {"学一", "教室食堂一", "教师食堂二", "雁南", "雁北"};
    shared_ptr<Building> buildings[5];
    for (int i = 0; i < 5; i++) {
        auto b1 = std::make_shared<Building>(building_names[i]);
        auto nid = mh.add(b1);
        b1->set_id(nid);
        buildings[i] = b1;
    }

    auto p = static_pointer_cast<Path>(mh.get(pid));
    auto con1 = mh.connect(buildings[2u], p);
    auto con2 = mh.connect(buildings[1u], p);
    ASSERT_EQ(con1.first, 3u);
    ASSERT_EQ(con2.first, 2u);
    auto x1 = static_pointer_cast<Building>(mh.get(3u));
    auto j1 = static_pointer_cast<Building>(mh.get(2u));
    // get the first connection path in x1
    auto con = *(x1->get_connections().begin());
    ASSERT_EQ(mh.get(con)->get_id(), p->get_id());
}

TEST(courseTest, testCourseParser) {
    string course_names[5] = {"2019211318班-数据结构课程设计-周四8:00", "2019211318班-数据结构课程设计-周四9:00",
                              "2019211318班-数据结构课程设计-周四10:00", "2019211318班-数据结构课程设计-周四11:00",
                              "2019211318班-数据结构课程设计-周四12:00"};
    Id physicalId[5] = {2, 4, 6, 7, 8};
    shared_ptr<Course> courses[5];
    CourseHub pornhub;
    for (auto i = 0; i < 5; i++) {
        auto a1 = std::make_shared<Course>(course_names[i]);
        pornhub.add(a1, physicalId[i]);
    }
    auto test = std::make_shared<Course>("2019211318班-数据结构课程设计-周四10:00");
    EXPECT_EQ(pornhub.getPhysicalId(test), 6) << "the right value";
    EXPECT_EQ(pornhub.remove(test), 6) << "the right value";
    test = std::make_shared<Course>("2019211318班-数据结构课程设计-周四10:00");
    EXPECT_EQ(pornhub.getPhysicalId(test), -1);
    EXPECT_EQ(pornhub.remove(test), -1);
}