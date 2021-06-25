#pragma once
#include "RigidBody.h"
#include <vector>

class ArbiterKey
{
public:
    ArbiterKey(std::shared_ptr<RigidBody> const &Body1, std::shared_ptr<RigidBody> const &Body2);

private:
    std::shared_ptr<RigidBody> Body1;
    std::shared_ptr<RigidBody> Body2;

    // used by std::map by the World class
    inline friend bool operator<(ArbiterKey const &a1, ArbiterKey const &a2)
    {
        if (a1.Body1 < a2.Body1)
            return true;

        if (a1.Body1 == a2.Body1 && a1.Body2 < a2.Body2)
            return true;

        return false;
    }
};

class Arbiter
{
public:
    Arbiter(std::shared_ptr<RigidBody> const& body1, std::shared_ptr<RigidBody> const& body2);
    Arbiter() = default;
    void PreStep(float invDt);
    void ApplyImpulse() const;
    bool InContact() const;
    std::vector<ContactPoint> const& GetContacts() const {return ContactManifold;}

private:
    std::shared_ptr<RigidBody> Body1;
    std::shared_ptr<RigidBody> Body2;
    std::vector<ContactPoint> ContactManifold;
    float Friction;
};
