#include <gtest/gtest.h>

#include "Entities/EntityManager.hpp"

using namespace Pixf::Core::Entities;

struct TransformComponent final : Component {
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;

    TransformComponent() = default;
    TransformComponent(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct HealthComponent final : Component {
    int health = 100;

    HealthComponent() = default;
    explicit HealthComponent(int h) : health(h) {}

    bool operator==(const HealthComponent &other) const { return this->health == other.health; }
};

class EntityManagerTest : public testing::Test {
protected:
    EntityManager entityManager;

    void TearDown() override { entityManager.Clear(); }
};

TEST_F(EntityManagerTest, CreateEntity_EntityIdsAreCorrect) {
    const Entity entity1 = entityManager.CreateEntity();
    const Entity entity2 = entityManager.CreateEntity();

    ASSERT_EQ(entity1.GetId(), 1);
    ASSERT_EQ(entity2.GetId(), 2);

    entityManager.DestroyEntity(entity1);

    const Entity entity3 = entityManager.CreateEntity();

    ASSERT_EQ(entity3.GetId(), 1);
}

TEST_F(EntityManagerTest, CreateEntity_EntityVersionsAreCorrect) {
    const Entity entity1 = entityManager.CreateEntity();

    ASSERT_EQ(entity1.GetVersion(), 0);

    entityManager.DestroyEntity(entity1);
    const Entity entity2 = entityManager.CreateEntity();

    ASSERT_EQ(entity2.GetVersion(), 1);
}

TEST_F(EntityManagerTest, AddComponent_ComponentCanBeRetrieved) {
    const Entity entity = entityManager.CreateEntity();
    entityManager.AddComponent<TransformComponent>(entity);

    EXPECT_TRUE(entityManager.HasComponent<TransformComponent>(entity));

    const auto result = entityManager.GetComponent<TransformComponent>(entity);
    ASSERT_TRUE(result.IsSuccess());

    const auto component = result.Unwrap();
    ASSERT_NE(component, nullptr);
    EXPECT_FLOAT_EQ(component->x, 0.0F);
}

TEST_F(EntityManagerTest, AddComponent_WithData_StoresCorrectData) {
    const Entity entity = entityManager.CreateEntity();

    const TransformComponent transform(1.0F, 2.0F, 3.0F);
    entityManager.AddComponent<TransformComponent>(entity, transform);
    const auto result = entityManager.GetComponent<TransformComponent>(entity);

    ASSERT_TRUE(result.IsSuccess());
    const auto component = result.Unwrap();
    component->x = 1.0F;
    component->y = 2.0F;
    component->z = 3.0F;

    EXPECT_FLOAT_EQ(component->x, 1.0F);
    EXPECT_FLOAT_EQ(component->y, 2.0F);
    EXPECT_FLOAT_EQ(component->z, 3.0F);
}

TEST_F(EntityManagerTest, RemoveComponent_ComponentNoLongerExists) {
    const Entity entity = entityManager.CreateEntity();
    entityManager.AddComponent<TransformComponent>(entity);

    EXPECT_TRUE(entityManager.HasComponent<TransformComponent>(entity));

    entityManager.RemoveComponent<TransformComponent>(entity);

    EXPECT_FALSE(entityManager.HasComponent<TransformComponent>(entity));
}

TEST_F(EntityManagerTest, DestroyEntity_ComponentsGetCleared) {
    const Entity entity = entityManager.CreateEntity();
    entityManager.AddComponent<TransformComponent>(entity);

    entityManager.DestroyEntity(entity);

    EXPECT_FALSE(entityManager.HasComponent<TransformComponent>(entity));
}

TEST_F(EntityManagerTest, MultipleComponents_AllCanBeRetrieved) {
    const Entity entity = entityManager.CreateEntity();

    entityManager.AddComponent<TransformComponent>(entity);
    entityManager.AddComponent<HealthComponent>(entity);

    EXPECT_TRUE(entityManager.HasComponent<TransformComponent>(entity));
    EXPECT_TRUE(entityManager.HasComponent<HealthComponent>(entity));

    const auto transform = entityManager.GetComponent<TransformComponent>(entity);
    const auto health = entityManager.GetComponent<HealthComponent>(entity);

    ASSERT_TRUE(transform.IsSuccess());
    ASSERT_TRUE(health.IsSuccess());
}

TEST_F(EntityManagerTest, GetComponent_NonExistentComponent_ReturnsError) {
    const Entity entity = entityManager.CreateEntity();

    entityManager.RegisterComponent<TransformComponent>();

    const auto result = entityManager.GetComponent<TransformComponent>(entity);

    EXPECT_TRUE(result.IsError());
    EXPECT_EQ(result.UnwrapError(), ComponentError::NotFound);
}

TEST_F(EntityManagerTest, GetComponent_NonRegisteredComponent_ReturnsError) {
    const Entity entity = entityManager.CreateEntity();

    const auto result = entityManager.GetComponent<TransformComponent>(entity);

    EXPECT_TRUE(result.IsError());
    EXPECT_EQ(result.UnwrapError(), ComponentError::NotRegistered);
}


TEST_F(EntityManagerTest, Query_ReturnsCorrectComponents) {
    const Entity entity1 = entityManager.CreateEntity();
    const Entity entity2 = entityManager.CreateEntity();

    const HealthComponent health(1);

    entityManager.AddComponent<HealthComponent>(entity1, health);
    entityManager.AddComponent<TransformComponent>(entity2);

    auto result = entityManager.Query<HealthComponent>();

    EXPECT_TRUE(result.IsSuccess());
    EXPECT_EQ(*result.Unwrap().at(entity1.GetId()), health);
}
