#ifndef maid2_graphics_modelmqo_h
#define maid2_graphics_modelmqo_h

/*!
    @file
    @brief  メタセコイアからモデルを構築するクラス
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/jobcachetemplate.h"
#include"../auxiliary/string.h"

#include"texture2d.h"
#include"vertex.h"
#include"index.h"
#include<boost/smart_ptr.hpp>

namespace Maid
{
  struct MQOSCENE
  {
    COLOR_R32G32B32A32F Ambient; //  全体の明るさ
  };

  struct MQOMATERIAL
  {
    COLOR_R32G32B32A32F Color;
    float Diffuse;
    float Ambient;
    float Specular;
  	float Emissive;
    float Power;
    Texture2D Texture;
    Texture2D Bump;
  };

  struct MQOOBJECT
  {
    std::vector<boost::shared_ptr<MQOOBJECT> >  Child;

    SIZE3DF Scale;
    float RotateHedding;//  Y軸回転(ラジアン度)
    float RotatePitch;  //  X軸回転
    float RotateBank;   //  Z軸回転
    VECTOR3DF Trans;

    Vertex Point;
    Vertex Normal;
    Vertex UV;
    Vertex Color;

    struct PRIMITIVE
    {
      enum
      {
        MATERIAL_NONE = -1, //  マテリアルが設定されていない
      };

      Index Index;
      int   MaterialNo;
    };
    std::vector<PRIMITIVE> Primitive;
  };



  namespace KEEPOUT
  {
    class MQOInput : public IJobInput
    {
    public:
      MQOInput(){}
      MQOInput( const String& name )
        :FileName(name){}
      String FileName;
    };

    inline bool operator < ( const MQOInput& lha, const MQOInput& rha ) 
    {
      return lha.FileName < rha.FileName; 
    }

    class MQOOutput : public IJobOutput
    {
    public:
      MQOSCENE  Scene;
      std::vector<MQOMATERIAL> Material;
      std::vector<boost::shared_ptr<MQOOBJECT> >   Object;
    };

    class MQOFunction;

  }

  class ModelMQO
  {
  public:
    void Load( const String& FileName );
    void Delete();
    bool IsLoading()const;


    const MQOSCENE& GetScene() const;
    const std::vector<MQOMATERIAL>& GetMaterialList() const;
    const std::vector<boost::shared_ptr<MQOOBJECT> >& GetObjectList() const;


  private:
    typedef JobCacheTemplate<KEEPOUT::MQOInput,KEEPOUT::MQOFunction, KEEPOUT::MQOOutput> CACHE;
    CACHE m_Cache;
  };	
}

#endif