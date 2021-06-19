#include "../headers/Arbiter.h"

Arbiter::Arbiter(std::shared_ptr<RigidBody> const& body1, std::shared_ptr<RigidBody> const& body2)
: Body1(body1), Body2(body2)
{
}

void Arbiter::PreStep(float invDt)
{
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
        kNormal += Body1->InvI * (r1.Dot(r1) - rn1 * rn1) + Body2->InvI * (r2.Dot(r2) + rn2 * rn2);
        c.massNormal = 1.0f / kNormal;

        // effective tangent mass
        Vec2 tangent = -c.normal.Perp();
        float rt1 = r1.Dot(tangent);
        float rt2 = r2.Dot(tangent);
        float kTangent = Body1->InvMass + Body2->InvMass;
        kTangent += Body1->InvI * (r1.Dot(r1) - rt1 * rt1) + Body2->InvI * (r2.Dot(r2) + rt2 * rt2);
        c.massTangent = 1.0f / kTangent;
        
        // bias but will leave that for now - invDt is used here

        // accumulateImpulses? will leave that for now too 
    }
}

void Arbiter::ApplyImpulse(float invDt)
{
    PreStep(invDt);

    for (auto& c : ContactManifold)
    {
        Vec2 r1 = c.point - Body1->Position;
        Vec2 r2 = c.point - Body2->Position;

        // relative velocity at the contact point
        Vec2 relV = Body2->LinearVelocity + Cross(Body2->AngularVelocity, r2) - Body1->LinearVelocity - Cross(Body1->AngularVelocity, r2);

        // Compute normal impulse
        float normalImp = relV.Dot(c.normal); 

        float dPn = c.massNormal * (-normalImp + c.bias); 

        // something about accumulating impulses here too that will come back to
        dPn = std::max(dPn, 0.0f);

        // apply contact impulse
        Vec2 Pn = dPn * c.normal;

        Body1->LinearVelocity -= Body1->InvMass * Pn;
        Body1->AngularVelocity -= Body1->InvI * Cross(r1, Pn);

        Body2->LinearVelocity += Body2->InvMass * Pn;
        Body2->AngularVelocity += Body2->InvI * Cross(r2, Pn);

        // relative velocity at the contact point 
        relV = Body2->LinearVelocity + Cross(Body2->AngularVelocity, r2) - Body1->LinearVelocity - Cross(Body1->AngularVelocity, r2);
        
        Vec2 tangent = -c.normal.Perp();
        float relVt = relV.Dot(tangent);
        float dPt = c.massTangent * (-relVt);

        // accumulate impulse stuff again

        float maxPt = Friction * dPn;
        dPt = Clamp(dPt, -maxPt, maxPt);

        // apply contact impulse
        Vec2 Pt = dPt * tangent;

        Body1->LinearVelocity -= Body1->InvMass * Pt;
        Body1->AngularVelocity -= Body1->InvI * Cross(r1, Pt);

        Body2->LinearVelocity += Body2->InvMass * Pt;
        Body2->AngularVelocity += Body2->InvI * Cross(r2, Pt);
    }

}