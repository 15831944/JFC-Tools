using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using ICSharpCode.SharpZipLib.Zip;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;


namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mZip
    {
        public static void GenerateZip(string inputPath, string targetLocation, [Optional][DateTimeConstant(627667488000000000L)] DateTime searchByDate, string searchPattern = "*.*", SearchOption searchOption = SearchOption.AllDirectories)
        {
            try
            {
                string[] files = Directory.GetFiles(inputPath, searchPattern, searchOption);
                List<string[]> list = new List<string[]>();
                if (Strings.StrComp(Directory.GetDirectoryRoot(inputPath), inputPath, CompareMethod.Text) < 0)
                {
                    inputPath = Directory.GetParent(inputPath).FullName;
                }
                int length = inputPath.Length;
                string[] array = files;
                foreach (string text in array)
                {
                    if (DateTime.Compare(searchByDate, new DateTime(627667488000000000L)) > 0)
                    {
                        if (DateTime.Compare(searchByDate, File.GetLastWriteTime(text)) < 0)
                        {
                            list.Add(new string[2]
                            {
                            text,
                            text.Substring(length)
                            });
                        }
                    }
                    else
                    {
                        list.Add(new string[2]
                        {
                        text,
                        text.Substring(length)
                        });
                    }
                }
                if (File.Exists(targetLocation))
                {
                    File.Delete(targetLocation);
                }
                FileStream baseOutputStream = new FileStream(targetLocation, FileMode.CreateNew);
                ZipOutputStream zipOutputStream = new ZipOutputStream(baseOutputStream);
                zipOutputStream.Password = "";
                foreach (string[] item in list)
                {
                    string name = item[1];
                    string obj = item[0];
                    Strings.InStrRev(obj, "\\");
                    byte[] array2 = File.ReadAllBytes(obj);
                    ZipEntry zipEntry = new ZipEntry(ZipEntry.CleanName(name));
                    File.GetCreationTime(obj);
                    File.GetLastAccessTime(obj);
                    DateTime dateTime = (zipEntry.DateTime = File.GetLastWriteTime(obj));
                    zipEntry.IsUnicodeText = true;
                    zipOutputStream.PutNextEntry(zipEntry);
                    zipOutputStream.Write(array2, 0, array2.Length);
                    zipOutputStream.CloseEntry();
                }
                zipOutputStream.Finish();
                zipOutputStream.Close();
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                ProjectData.ClearProjectError();
            }
        }
    }
}