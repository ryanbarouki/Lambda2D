#include "../headers/Arbiter.h"
#include "../headers/NarrowPhase.h"
#include "../headers/World.h"
#include <cmath>
#include <iostream>

ArbiterKey::ArbiterKey(std::shared_ptr<RigidBody> const& body1, std::shared_ptr<RigidBody> const& body2)
{
    if (body1 < body2)
    {
        Body1 = body1;
        Body2 = body2;
    }
    else
    {
        Body1 = body2;
        Body2 = body1;
    }
}

Arbiter::Arbiter(std::shared_ptr<RigidBody> const& body1, std::shared_ptr<RigidBody> const& body2)
: Body1(body1), Body2(body2)
{
    if (body1 < body2)
    {
        Body1 = body1;
        Body2 = body2;
    }
    else
    {
        Body1 = body2;
        Body2 = body1;
    }


    // THIS IS BAD AND SHOULD BE FIXED ASAP
    ContactManifold = NarrowPhase::FindContactPoints(dynamic_cast<Polygon const&>(*Body1->Shape), dynamic_cast<Polygon const&>(*Body2->Shape));
    Friction = sqrtf(Body1->Friction * Body2->Friction);
}

void Arbiter::PreStep(float invDt) 
{
    const float allowedPenetration = 0.01f;
    float biasFactor = World::positionCorrection ? 0.2f : 0.0f; // 0.2f
    for (auto& c : ContactManifold)
    {
        // vectors from centre of Body to contact point
        Vec2 r1 = c.point - Body1->Position;
        Vec2 r2 = c.point - Body2->Position;

        // compute the effective masses at the contact point
        // follow PreStep in Arbiter.cpp Box2dLite
        // effective normal mass
        float rn1 = r1.Dot(c.normal);
        float rn2 = r2.Dot(c.normal);
        float kNormal = Body1->InvMass + Body2->InvMass;
        kNormal += Body1->InvI * (r1.Dot(r1) - rn1 * rn1) + Body2->InvI * (r2.Dot(r2) - rn2 * rn2);
        c.massNormal = 1.0f / kNormal;

        // effective tangent mass
        Vec2 tangent = -c.normal.Perp();
        float rt1 = r1.Dot(tangent);
        float rt2 = r2.Dot(tangent);
        float kTangent = Body1->InvMass + Body2->InvMass;
        kTangent += Body1->InvI * (r1.Dot(r1) - rt1 * rt1) + Body2->InvI * (r2.Dot(r2) - rt2 * rt2);
        c.massTangent = 1.0f / kTangent;
        
        c.bias = -biasFactor * invDt * std::min(0.0f, -c.depth + allowedPenetration);

        // accumulateImpulses? will leave that for now too 
        if (World::accumulateImpulses)
        {
            Vec2 P = c.Pn * c.normal + c.Pt * tangent;

            Body1->LinearVelocity -= Body1->InvMass * P;
            Body1->AngularVelocity -= Body1->InvI * Cross(r1, P);

            Body2->LinearVelocity += Body2->InvMass * P;
            Body2->AngularVelocity += Body2->InvI * Cross(r2, P);
        }
    }
}

void Arbiter::ApplyImpulse()
{
    for (auto& c : ContactManifold)
    {
        Vec2 r1 = c.point - Body1->Position;
        Vec2 r2 = c.point - Body2->Position;

        // relative velocity at the contact point
        Vec2 relV = Body2->LinearVelocity + Cross(Body2->AngularVelocity, r2) - Body1->LinearVelocity - Cross(Body1->AngularVelocity, r1);

        // Compute normal impulse
        float normalImp = relV.Dot(c.normal); 

        float dPn = c.massNormal * (-normalImp + c.bias); 

        // something about accumulating impulses here too that will come back to
        if (World::accumulateImpulses)
        {
            float Pn0 = c.Pn;
            c.Pn = std::max(Pn0 + dPn, 0.0f);
            dPn = c.Pn - Pn0;
        }
        else
        {
            dPn = std::max(dPn, 0.0f);
        }

        // apply contact impulse
        Vec2 Pn = dPn * c.normal;

        Body1->LinearVelocity -= Body1->InvMass * Pn;
        Body1->AngularVelocity -= Body1->InvI * Cross(r1, Pn);

        Body2->LinearVelocity += Body2->InvMass * Pn;
        Body2->AngularVelocity += Body2->InvI * Cross(r2, Pn);

        // relative velocity at the contact point 
        relV = Body2->LinearVelocity + Cross(Body2->AngularVelocity, r2) - Body1->LinearVelocity - Cross(Body1->AngularVelocity, r1);
        
        Vec2 tangent = -c.normal.Perp();
        float relVt = relV.Dot(tangent);
        float dPt = c.massTangent * (-relVt);

        // accumulate impulse stuff again
        if (World::accumulateImpulses)
        {
            float maxPt = Friction * c.Pn;

            float oldTangentImpulse = c.Pt;
            c.Pt = Clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
            dPt = c.Pt - oldTangentImpulse;
        }
        else
        {
            float maxPt = Friction * dPn;
            dPt = Clamp(dPt, -maxPt, maxPt);
        }

        // apply contact impulse
        Vec2 Pt = dPt * tangent;

        Body1->LinearVelocity -= Body1->InvMass * Pt;
        Body1->AngularVelocity -= Body1->InvI * Cross(r1, Pt);

        Body2->LinearVelocity += Body2->InvMass * Pt;
        Body2->AngularVelocity += Body2->InvI * Cross(r2, Pt);
    }

}

bool Arbiter::InContact() const
{
    return !ContactManifold.empty();
}

void Arbiter::UpdateContacts(std::vector<ContactPoint> const& newContacts)
{
    std::vector<ContactPoint> newManifold;
    int numContacts = newContacts.size();
    
    for (int i = 0; i < numContacts; ++i)
    {
        auto cNew = newContacts[i];
        int k = -1;
        for (int j = 0; j < ContactManifold.size(); ++j)
        {
            auto cOld = ContactManifold[j];
            if (cNew.edgeNum == cOld.edgeNum)
            {
                k = j;
                break;
            }
        }

        if (k > -1)
        {
            ContactPoint c = cNew;
            ContactPoint cOld = ContactManifold[k];

            // warm starting
            c.Pn = cOld.Pn;
            c.Pt = cOld.Pt;
            c.Pnb = cOld.Pnb;

            newManifold.push_back(c);
        }
        else
        {
            newManifold.push_back(newContacts[i]);
        }
    }

    ContactManifold = newManifold;
}