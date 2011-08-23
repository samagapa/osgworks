/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * osgWorks is (C) Copyright 2009-2011 by Kenneth Mark Bryden
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef __SHORT_EDGE_OP_H__
#define __SHORT_EDGE_OP_H__

#include <osgwTools/Export.h>
#include <osgwTools/GeometryOperation.h>
#include <osg/Geode>
#include <osg/Geometry>

namespace osgwTools {


/** \brief A geometry reduction tool capable of removing edge vertices based on a maximum edge length expressed as a percentage of the smallest model bounding box dimension.

For more information, see \ref geomopt
*/
class OSGWTOOLS_EXPORT ShortEdgeOp : public GeometryOperation
{
public:

    ShortEdgeOp(double sampleRatio=1.0, double maximumError=FLT_MAX, double maxFeature=.1, unsigned int maxSteps=UINT_MAX);

    ShortEdgeOp( const ShortEdgeOp& rhs, const osg::CopyOp& copyOp=osg::CopyOp::SHALLOW_COPY );
    META_Object(osgwTools,ShortEdgeOp);


    void setSampleRatio(float sampleRatio) { _sampleRatio = sampleRatio; }
    float getSampleRatio() const { return _sampleRatio; }

    void setMaxFeature(float maxFeature) { _maxFeature = maxFeature; }
    float getMaxFeature() const { return _maxFeature; }

    void setMaximumError(float error) { _maximumError = error; }
    float getMaximumError() const { return _maximumError; }

    void setDoTriStrip(bool on) { _triStrip = on; }
    bool getDoTriStrip() const { return _triStrip; }

    void setSmoothing(bool on) { _smoothing = on; }
    bool getSmoothing() const { return _smoothing; }

    void setIgnoreBoundaries(bool setting){_ignoreBoundaries = setting;}
    bool getIgnoreBoundaries() const {return _ignoreBoundaries;}

    void setMinPrimitives(unsigned int minPrim){_minPrim = minPrim;}
    unsigned int getMinPrimitives() const { return _minPrim;}

    void setMaxSteps(unsigned int maxSteps){_maxSteps = maxSteps;}

    /** \brief Application control mechanism to continue or halt decimation.
    */
    class ContinueDecimationCallback : public osg::Referenced
        {
            public:
                /** Returns True if mesh is to continue being decimated. Returns False to stop decimation.*/
                virtual bool continueDecimation(const ShortEdgeOp& decimator, float nextError, unsigned int numOriginalPrimitives, unsigned int numRemainingPrimitives) const
                {
                    return decimator.continueDecimationImplementation(nextError, numOriginalPrimitives, numRemainingPrimitives);
                }
            
            protected:
                virtual ~ContinueDecimationCallback() {}
        };
     void setContinueDecimationCallback(ContinueDecimationCallback* cb) { _continueDecimationCallback = cb; }
     ContinueDecimationCallback* getContinueDecimationCallback() { return _continueDecimationCallback.get(); }
     const ContinueDecimationCallback* getContinueDecimationCallback() const { return _continueDecimationCallback.get(); }
        
        
     bool continueDecimation(float nextError, unsigned int numOriginalPrimitives, unsigned int numRemainingPrimitives) const
     {
         if (_continueDecimationCallback.valid()) return _continueDecimationCallback->continueDecimation(*this, nextError, numOriginalPrimitives, numRemainingPrimitives);
         else return continueDecimationImplementation(nextError, numOriginalPrimitives, numRemainingPrimitives);
     }
     virtual bool continueDecimationImplementation(float nextError, unsigned int numOriginalPrimitives, unsigned int numRemainingPrimitives) const
     {
         return ((float)numRemainingPrimitives > ((float)numOriginalPrimitives) * getSampleRatio()) && nextError<=getMaximumError();
        
     }

    virtual osg::Geometry* operator()( osg::Geometry& geom )
    {
        decimate( geom );

        return( &geom );
    }
    
     void decimate(osg::Geometry& geometry);

protected:
    
   double _sampleRatio;
   double _maxFeature;
   float _maximumError;
   bool  _triStrip;
   bool  _smoothing;
   bool  _ignoreBoundaries;
   unsigned int   _minPrim;
   unsigned int   _maxSteps;

    osg::ref_ptr<ContinueDecimationCallback> _continueDecimationCallback;
};

}

#endif