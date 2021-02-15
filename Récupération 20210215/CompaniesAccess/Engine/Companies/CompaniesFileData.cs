using AR7Common;
using ARDataProvider;
using RadioEngine;
using System;
using System.Collections.Generic;
using System.IO;

namespace CompaniesAccess
{
    public class CompaniesFileData
    {




        public Dictionary<String, ARUserFileTable> Files { get; private set;}


        public CompaniesFileData()
        {

            this.Files = new Dictionary<string, ARUserFileTable>();
        }

        public void Initialize()
        {

            PathAInfo lPathAUserTableFile = RuntimeInfoManager.Singleton.Get<PathAInfo>("userTableFile");
            if (lPathAUserTableFile != null)
            {
                String l_directory= "";
                //determine if it is a file 
                if (File.Exists(lPathAUserTableFile.Path))
                {
                    l_directory = Path.GetDirectoryName(lPathAUserTableFile.Path);
                }
                else
                {
                    //Directory validity has already been checked by PathAInfo
                    l_directory = lPathAUserTableFile.Path;
                }
                String[] l_files = Directory.GetFiles(l_directory,"*.tbl");
                foreach(String i_file in l_files)
                {
                    this.LoadFiles(i_file);

                }

            }
        }


        public void LoadFiles(String _path)
        {

            
            if (_path != null && File.Exists(_path) )
            {
                string l_path = _path.ToLower();

                //fixe chain choosen by Olivier
                if (l_path.Contains("tableutilisateur"))
                    {

                    String l_fileName = Path.GetFileNameWithoutExtension(l_path);


                    int l_indexchar = l_fileName.IndexOf('_');
                    String l_user = l_fileName.Substring(l_indexchar + 1);
                    if (!this.Files.ContainsKey(l_user))
                    {


                        ARUserFileTable l_userFileTable = new ARUserFileTable(_path);
                        if (l_userFileTable.Lines.Count > 0)
                        {
                            this.Files.Add(l_user, l_userFileTable);
                        }
                    }
                    else
                        ErrorManagement.Singleton.Error(ErrorManagement.TypeError.MINOR, "tableuser" + l_user);
                }
            }


        }
    }
}
