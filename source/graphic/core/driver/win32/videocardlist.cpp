#include"videocardlist.h"

namespace Maid
{

int VideoCardList::GetVideoCardCount()
{
  return 1;
}
VideoCardListBase::VIDEOCARDINFO VideoCardList::GetVideoCardInfo( int no )
{
  VIDEOCARDINFO info;
  info.Name = MAIDTEXT("Direct3D9");

  return info;
}

SPVIDEOCARD VideoCardList::CreateVideoCard( int No, const VideoCardListBase::CREATEPARAM& Info )
{
  return SPVIDEOCARD();
}

}



