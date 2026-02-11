#include <gtest/gtest.h>

#include "Ecs/Registry.hpp"
#include "Ecs/Schedule.hpp"
#include "Ecs/Storage.hpp"

using namespace Flock::Ecs;

TEST(Entities, Storage) {
    // Arrange
    Storage<int> storage{};

    // Act
    storage.Insert(4, 2);
    storage.Insert(1, 5);
    storage.Insert(2, 3);
    storage.Remove(2);

    // Assert
    ASSERT_TRUE(storage.Has(4));
    ASSERT_TRUE(storage.Has(1));
    ASSERT_FALSE(storage.Has(2));
    ASSERT_FALSE(storage.Has(3));

    ASSERT_EQ(storage.Get(4), 2);
    ASSERT_EQ(storage.Get(1), 5);
    ASSERT_EQ(storage.Get(2), std::nullopt);
    ASSERT_EQ(storage.Get(3), std::nullopt);
}

TEST(Entities, Registry) {
    // Arrange
    Registry registry{};

    // Act
    const Entity dead = registry.Create();
    registry.Destroy(dead);
    const Entity e1 = registry.Create();
    const Entity e2 = registry.Create();

    registry.AddComponent<int>(e1, 1);

    registry.AddComponent<int>(e2, 1);
    registry.AddComponent<char>(e2, 'B');
    registry.GetComponent<int>(e2)->get() = 2;

    bool hasAllComps = registry.HasAllComponents<int, char>(e2);
    bool hasAnyComps = registry.HasAnyComponents<int, char>(e1);

    // Assert
    ASSERT_FALSE(registry.IsAlive(dead));
    ASSERT_TRUE(registry.IsAlive(e1));
    ASSERT_TRUE(registry.IsAlive(e2));

    ASSERT_EQ(e1.id, dead.id);
    ASSERT_EQ(e1.version, 1);

    ASSERT_TRUE(registry.HasComponent<int>(e1));
    ASSERT_FALSE(registry.HasComponent<char>(e1));
    ASSERT_TRUE(registry.HasComponent<int>(e2));
    ASSERT_TRUE(registry.HasComponent<char>(e2));

    ASSERT_TRUE(hasAllComps);
    ASSERT_TRUE(hasAnyComps);

    ASSERT_EQ(registry.GetComponent<int>(e1).value(), 1);
    ASSERT_EQ(registry.GetComponent<char>(e1), std::nullopt);
    ASSERT_EQ(registry.GetComponent<int>(e2).value(), 2);
    ASSERT_EQ(registry.GetComponent<char>(e2).value(), 'B');
}

TEST(Entities, RegistryForEach) {
    // Arrange
    Registry registry{};

    for (int i = 0; i < 100; i++) {
        const Entity e = registry.Create();
        registry.AddComponent<int>(e, i);
    }

    // Act
    registry.ForEach<int>([](int &integer) {
        integer++;
    });

    // Assert
    for (EntityId i = 0; i < 100; i++) {
        const auto e = Entity{.id = i, .version = 0};
        ASSERT_EQ(registry.GetComponent<int>(e), i + 1);
    }
}

TEST(Entities, Schedule) {
    // Arrange
    Registry registry{};
    Schedule schedule{};

    const Entity entity = registry.Create();
    registry.AddComponent<int>(entity, 1);

    // Act
    schedule.AddSystem(Stage::Startup, [&](Registry &reg) {
        reg.GetComponent<int>(entity)->get()--;
    });

    schedule.AddSystem(Stage::Update, [&](Registry &reg) {
        reg.GetComponent<int>(entity)->get()++;
    });

    schedule.Execute(Stage::Update, registry);
    schedule.Execute(Stage::Startup, registry);
    schedule.Execute(Stage::Update, registry);

    // Assert
    ASSERT_EQ(registry.GetComponent<int>(entity)->get(), 2);
}
