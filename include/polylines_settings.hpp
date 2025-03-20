#pragma once
#include "lines_settings.hpp"

namespace lines {

    enum class PolyLineJoint {
        ROUND_JOINT = 0x00000000, // Joint with round shape
        BEVEL_JOINT = 0x00000001, // Joint with square shape
        MITER_JOINT = 0x00000002, // Joint with a miter
    };
    
    class PolylinesSettings : public LinesSettings
    {
    private:
        uint8_t mMiterLimit = mThickness * 2;
    
        PolyLineJoint mJoint = PolyLineJoint::ROUND_JOINT;
    
    public:
        PolylinesSettings() = default;
    
        PolyLineJoint getJoint() const { return mJoint; }
    
        void setMiterLimit(uint8_t miterLimit)
        {
            if (miterLimit < mThickness)
                assert(
                    (void("Miter limit must be greatest then thickness * 2"),
                     false));
            mMiterLimit = miterLimit;
        }
    
        void setJoint(PolyLineJoint joint) { mJoint = joint; }
    
        void bindUniform() const
        {
            uint32_t thickness_antialias_border_miterlimit =
                (0 | mThickness << 24 | mAntialias << 16 | mBorder << 8 |
                 mMiterLimit);
    
            uint32_t caps_joint_color =
                (0 | static_cast<uint8_t>(mLeftCap) << 6 |
                 static_cast<uint8_t>(mRigthCap) << 4 |
                 static_cast<uint8_t>(mJoint) << 2 |
                 static_cast<uint8_t>(mColorToUse));
    
            uint32_t data[] = {
                mGeneralColor,
                thickness_antialias_border_miterlimit,
                mBorderColor,
                caps_joint_color
            };
            bgfx::setUniform(mDataUH, data);
        }
    };
}