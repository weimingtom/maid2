


function GetDirectryName( path )
{
  local pos = path.len()-1
  
  while( true )
  {
    if( pos==0 ) { break; }
    if( path[pos]=='\\' || path[pos]=='/' )
    {
      return path.slice(0,pos)
    }
    --pos
  }
  
  return ""
}

function GetFileName( path )
{
  local pos = path.len()-1
  
  while( true )
  {
    if( pos<=0 ) { return path; }
    if( path[pos]=='\\' || path[pos]=='/' )
    {
      return path.slice(pos+1)
    }
    --pos
  }
  
  return ""
}


//!   新しくsquirrelファイルを加える
/*!
  @param  filename  [i ] 加えるファイル名

  @return 追加に成功したら true
          失敗したらfalse
 */
import_dir <- ""

function import( filename )
{
  //  filenameがディレクトリついてたりすると
  //  ソース側がなんか気持ち悪くなるので
  //  一時的に覚えておく
  local dir  = GetDirectryName(filename)
  local name = GetFileName(filename)
  local olddir = import_dir

  import_dir += dir

  local path = ""
  if( import_dir.len()==0 ) { path = name }
  else { path = import_dir + "/" + name }

  Import( path )
  
  import_dir = olddir

  return FUNCTIONRESULT.SUCCESS;
}


gameloop <- true;

function ExitGameLoop()
{
  gameloop = false;
}

function Run( routine )
{
  storage <- Storage()
  storage.Load( "storage.xml" )

  admin <- SceneAdmin();// この変数はC++側からもアクセスしてるので名前を変えないこと
  input <- KeyInput();

  admin.Initialize( routine, input );

  while( gameloop )
  {
    input.Update()
    admin.UpdateFrame();

    suspend(["nextframe"]);
  }

  admin.Finalize();
  storage.Save()
  delete input
  delete admin
  delete storage

  collectgarbage();
}

