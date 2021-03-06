//////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2017 EMC Corporation
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is EMC Corporation
///
/// @author Andrey Abramov
/// @author Vasiliy Nabatchikov
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"

#include "IResearch/Containers.h"

// -----------------------------------------------------------------------------
// --SECTION--                                                 setup / tear-down
// -----------------------------------------------------------------------------

struct ContainersSetup { };

// -----------------------------------------------------------------------------
// --SECTION--                                                        test suite
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief setup
////////////////////////////////////////////////////////////////////////////////

TEST_CASE("ContainersTest", "[iresearch][iresearch-containers]") {
  ContainersSetup s;
  UNUSED(s);

  SECTION("test_Hasher") {
    // ensure hashing of irs::bytes_ref is possible
    {
      typedef arangodb::iresearch::UnorderedRefKeyMapBase<irs::byte_type, int>::KeyHasher Hasher;
      Hasher hasher;
      irs::string_ref strRef("abcdefg");
      irs::bytes_ref ref = irs::ref_cast<irs::byte_type>(strRef);
      CHECK(false == (0 == hasher(ref)));
    }

    // ensure hashing of irs::string_ref is possible
    {
      typedef arangodb::iresearch::UnorderedRefKeyMapBase<char, int>::KeyHasher Hasher;
      Hasher hasher;
      irs::string_ref ref("abcdefg");
      CHECK(false == (0 == hasher(ref)));
    }
  }

  SECTION("test_UniqueHeapInstance") {
    {
      struct TestStruct {
      };

      // ensure copy works (different instance)
      {
        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance;
        auto* ptr = instance.get();

        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance1;
        auto* ptr1 = instance1.get();
        CHECK(false == (ptr == instance1.get()));
        instance1 = instance;
        CHECK(false == (ptr1 == instance1.get()));
        CHECK(false == (ptr == instance1.get()));

        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance2(instance);
        CHECK(false == (ptr == instance2.get()));
      }

      // ensure element copy works (different instance)
      {
        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance;
        auto* ptr = instance.get();

        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance1;
        auto* ptr1 = instance1.get();
        CHECK(false == (ptr == instance1.get()));
        instance1 = *instance;
        CHECK(true == (ptr1 == instance1.get()));
        CHECK(false == (ptr == instance1.get()));

        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance2(*instance);
        CHECK(false == (ptr == instance2.get()));
      }
    }

    {
      static size_t counter = 0;
      struct TestStruct {
        size_t id;
        TestStruct(): id(++counter) {}
        TestStruct(TestStruct&& other): id(other.id) {};
        TestStruct(TestStruct const&) = delete;
        TestStruct& operator=(TestStruct const&) = delete;
        TestStruct& operator=(TestStruct&& other) { id = other.id; return *this; }
      };

      // ensure move works (same instance)
      {
        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance;
        auto* ptr = instance.get();

        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance1;
        CHECK(false == (ptr == instance1.get()));
        instance1 = std::move(instance);
        CHECK(true == (ptr == instance1.get()));

        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance2(std::move(instance1));
        CHECK(true == (ptr == instance2.get()));
      }

      // ensure value move works (same instance)
      {
        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance;
        auto* ptr = instance.get();
        auto id = ptr->id;

        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance1;
        CHECK(false == (ptr == instance1.get()));
        CHECK(false == (id == instance1->id));
        instance1 = std::move(*instance);
        CHECK(true == (id == instance1->id));

        arangodb::iresearch::UniqueHeapInstance<TestStruct> instance2(std::move(*instance1));
        CHECK(true == (id == instance2->id));
      }
    }
  }

  SECTION("test_UnorderedRefKeyMap") {
    static size_t counter = 0;
    struct TestStruct {
      size_t id;
      int value;
      TestStruct(): id(++counter), value(-1) {}
      TestStruct(int v): id(++counter), value(v) {}
    };

    arangodb::iresearch::UnorderedRefKeyMap<char, TestStruct> map;
    CHECK(true == (0 == counter));
    CHECK(true == (map.empty()));
    CHECK(true == (0 == map.size()));

    // new element via operator
    {
      auto& value = map["abc"];
      CHECK(true == (1 == counter));
      CHECK(false == (map.empty()));
      CHECK(true == (1 == map.size()));
      CHECK(true == (1 == value.id));
      CHECK(true == (-1 == value.value));
    }

    // new element via emplace no args
    {
      auto itr = map.emplace("def");
      CHECK(true == (2 == counter));
      CHECK(false == (map.empty()));
      CHECK(true == (2 == map.size()));
      CHECK(true == (itr.second));
      CHECK(true == (irs::string_ref("def") == itr.first.key()));
      CHECK(true == (2 == itr.first.value().id));
      CHECK(true == (-1 == itr.first.value().value));
    }

    // new element via emplace with args
    {
      auto itr = map.emplace("ghi", 42);
      CHECK(true == (3 == counter));
      CHECK(false == (map.empty()));
      CHECK(true == (3 == map.size()));
      CHECK(true == (itr.second));
      CHECK(true == (irs::string_ref("ghi") == itr.first.key()));
      CHECK(true == (3 == itr.first.value().id));
      CHECK(true == (42 == itr.first.value().value));
    }

    // duplicate element via operator
    {
      auto& value = map["ghi"];
      CHECK(true == (3 == counter));
      CHECK(false == (map.empty()));
      CHECK(true == (3 == map.size()));
      CHECK(true == (3 == value.id));
      CHECK(true == (42 == value.value));
    }

    // duplicate element via emplace no args
    {
      auto itr = map.emplace("ghi");
      CHECK(true == (3 == counter));
      CHECK(false == (map.empty()));
      CHECK(true == (3 == map.size()));
      CHECK(false == (itr.second));
      CHECK(true == (irs::string_ref("ghi") == itr.first.key()));
      CHECK(true == (3 == itr.first.value().id));
      CHECK(true == (42 == itr.first.value().value));
    }

    // duplicate element via emplace with args
    {
      auto itr = map.emplace("def", 1234);
      CHECK(true == (3 == counter));
      CHECK(false == (map.empty()));
      CHECK(true == (3 == map.size()));
      CHECK(false == (itr.second));
      CHECK(true == (irs::string_ref("def") == itr.first.key()));
      CHECK(true == (2 == itr.first.value().id));
      CHECK(true == (-1 == itr.first.value().value));
    }

    // search via iterator
    {
      auto itr = map.find("ghi");
      CHECK(false == (map.end() == itr));
      CHECK(true == (irs::string_ref("ghi") == itr.key()));
      CHECK(true == (3 == itr.value().id));
      CHECK(true == (42 == itr.value().value));

      itr = map.find("xyz");
      CHECK(true == (map.end() == itr));
    }

    // search via pointer
    {
      auto ptr = map.findPtr("ghi");
      CHECK(false == (nullptr == ptr));
      CHECK(true == (3 == ptr->id));
      CHECK(true == (42 == ptr->value));

      ptr = map.findPtr("xyz");
      CHECK(true == (nullptr == ptr));
    }

    // validate iteration
    {
      std::set<std::string> expected({"abc", "def", "ghi"});

      for (auto& entry: map) {
        CHECK(true == (1 == expected.erase(entry.key())));
      }

      CHECK(true == (expected.empty()));
    }

    map.clear();
    CHECK(true == (0 == map.size()));
    CHECK(true == (map.begin() == map.end()));
  }

////////////////////////////////////////////////////////////////////////////////
/// @brief generate tests
////////////////////////////////////////////////////////////////////////////////

}

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------