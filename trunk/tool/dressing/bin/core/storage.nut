
class Storage
{
  Root = null
  FileName = null

  constructor()
  {
    Root = {}
    FileName = ""
  }

  function Save()
  {
    local ret = ::suspend( ["storagesave", FileName] )
  }
  
  function Load( filename )
  {
    local ret = ::suspend( ["storageload", filename] )
    Root = ret
    FileName = filename
  }

}

