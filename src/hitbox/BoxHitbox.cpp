#include "BoxHitbox.h"
#include "../Terrain.h"
#ifdef HITBOX_DEBUG
#include <ofGraphics.h>
#include <of3DGraphics.h>
#endif // HITBOX_DEBUG
BoxHitbox::BoxHitbox(Vector3D* position, Quaternion* orientation, const Vector3D& position_offset, float half_width, float half_height, float half_depth)
    : HitBoxPrimitive(position, orientation, position_offset),
    _half_width(half_width), _half_height(half_height), _half_depth(half_depth)
{
#ifdef HITBOX_DEBUG
    _points.setMode(OF_PRIMITIVE_POINTS);
    _points.disableIndices();
#endif
}

bool BoxHitbox::doCollideWithBox(const BoxHitbox* other_hitbox, std::array<CollisionData, 8>& collision_data)
{
    bool collision_detected = false;

    const std::array<Vector3D, 2> other_hitbox_point_on_face = { other_hitbox->_hitbox_corner[0], other_hitbox->_hitbox_corner[7] };

    for (size_t i = 0; i < std::size(_hitbox_corner); i++)
    {
        const Vector3D& q = _hitbox_corner[i];

        int face_penetration = 0;
        
        for (const Vector3D& normal : other_hitbox->_faces_normals)
        {
            for (int dir = 0; dir < 2; dir++)
            {
                Vector3D n(normal * (dir * 2 - 1));
                const Vector3D p(other_hitbox_point_on_face[1 - dir]);
                n.normalize();
                float t = Vector3D::dotProduct(n, q - p);
                if (t <= 0)
                {
                    if (std::abs(t) < collision_data[i]._penetration_distance)
                    {
                        collision_data[i]._penetration_distance = std::abs(t);
                        collision_data[i]._contact_normal = n;
                        collision_data[i]._impact_point = (q - t * n);
                    }
                    face_penetration++;
                }
                else
                {
                    goto NEXT_POINT;
                }
            }
        }

        NEXT_POINT:
        if (face_penetration == 6)
        {
            collision_data[i]._collision_detected = true;
            collision_detected = true;
        }
    }
    return collision_detected;
}

void BoxHitbox::update()
{
    _hitbox_corner = {
        _position_offset + Vector3D(_half_width,  _half_height,  _half_depth),
        _position_offset + Vector3D(_half_width,  _half_height, -_half_depth),
        _position_offset + Vector3D(_half_width, -_half_height,  _half_depth),
        _position_offset + Vector3D(_half_width, -_half_height, -_half_depth),
        _position_offset + Vector3D(-_half_width,  _half_height,  _half_depth),
        _position_offset + Vector3D(-_half_width,  _half_height, -_half_depth),
        _position_offset + Vector3D(-_half_width, -_half_height,  _half_depth),
        _position_offset + Vector3D(-_half_width, -_half_height, -_half_depth)
    };

#ifdef HITBOX_DEBUG
    _points.clear();
#endif

    for (Vector3D& p : _hitbox_corner)
    {
        _orientation->rotatePoint(p);
        p += *_position;
#ifdef HITBOX_DEBUG
        _points.addVertex(p);
#endif    
    }

    _faces_normals = {
        _orientation->getLookAtDir(),
        _orientation->getSideDir(),
        _orientation->getUpDir()
    };
}
bool BoxHitbox::doCollideWithTerrain(Terrain* terrain, std::array<CollisionData, 8>& collision_data)
{
    bool out = false;
    for (size_t i = 0; i < std::size(_hitbox_corner); i++)
    {
        const float terrain_height = terrain->getHeight(_hitbox_corner[i].x, _hitbox_corner[i].z);
        if (terrain_height > _hitbox_corner[i].y)
        {
            collision_data[i]._collision_detected = true;
            collision_data[i]._contact_normal = terrain->getNormal(_hitbox_corner[i].x, _hitbox_corner[i].z);
            collision_data[i]._impact_point.set(_hitbox_corner[i].x, terrain_height,_hitbox_corner[i].z);
            collision_data[i]._penetration_distance = terrain_height - _hitbox_corner[i].y;

            out = true;
        }
    }
    return out;
}
#ifdef HITBOX_DEBUG
void BoxHitbox::drawBox()
{
    ofSetColor(255, 0, 0, 120);
    ofDrawBox(_position_offset, 2 * _half_width, 2 * _half_height, 2 * _half_depth);

}
void BoxHitbox::drawCorner()
{
    ofSetColor(255, 255, 255, 255);
    _points.draw();
}
#endif // HITBOX_DEBUG
