﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ARParameter.Module.File
{
    /// <summary>
    /// Classe permettant de gérer les fichiers INI.
    /// </summary>
    public class IniFile
    {
        // API Windows
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileSection(string section, IntPtr lpReturnedString, int nSize, string lpFileName);

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileSectionNames(IntPtr lpszReturnBuffer, int nSize, string lpFileName);

        [DllImport("kernel32")]
        private static extern int WritePrivateProfileSection(string section, string lpString, string lpFileName);

        [DllImport("kernel32")]
        private static extern int WritePrivateProfileString(string section, string key, string lpString, string lpFileName);

        private string fileName;

        /// <summary>
        /// Obtient ou définit le chemin d'accès complet au fichier INI.
        /// </summary>
        public string Filename
        {
            get { return fileName; }
            set { fileName = value; }
        }

        /// <summary>
        /// Initialize une instance de <see cref="Ini"/>.
        /// </summary>
        /// <param name="lpFileName">Chemin d'accès complet au fichier INI.</param>
        public IniFile(string file)
        {
            this.fileName = file;
        }

        /// <summary>
        /// Supprime une section ainsi que toutes les valeurs qu'elle contient.
        /// </summary>
        /// <param name="section">Nom de la section.</param>
        public int RemoveSection(string section)
        {
            return WritePrivateProfileSection(section, null, fileName);
        }

        /// <summary>
        /// Supprime une valeur.
        /// </summary>
        /// <param name="section">Nom de la section.</param>
        /// <param name="key">Nom de la valeur.</param>
        public int RemoveString(string section, string key)
        {
            return WritePrivateProfileString(section, key, null, fileName);
        }

        /// <summary>
        /// Ecrit une valeur dans une section.
        /// </summary>
        /// <param name="section">Nom de la section.</param>
        /// <param name="key">Nom de la valeur.</param>
        /// <param name="lpString">Valeur.</param>
        public int WriteString(string section, string key, string lpString)
        {
            return WritePrivateProfileString(section, key, lpString, fileName);
        }

        /// <summary>
        /// Obtient la valeur d'une section.
        /// </summary>
        /// <param name="section">Nom de la section.</param>
        /// <param name="key">Nom de la valeur.</param>
        public string ReadString(string section, string key)
        {
            const int bufferSize = 255;
            StringBuilder temp = new StringBuilder(bufferSize);
            GetPrivateProfileString(section, key, "", temp, bufferSize, fileName);
            return temp.ToString();
        }

        /// <summary>
        /// Obtient l'ensemble des valeurs d'une section.
        /// </summary>
        /// <param name="section">Nom de la section.</param>
        public string[] ReadSection(string section)
        {
            const int bufferSize = 2048;

            StringBuilder returnedString = new StringBuilder();

            IntPtr pReturnedString = Marshal.AllocCoTaskMem(bufferSize);
            try
            {
                int bytesReturned = GetPrivateProfileSection(section, pReturnedString, bufferSize, fileName);

                // bytesReturned -1 pour retirer le dernier \0
                for (int i = 0; i < bytesReturned - 1; i++)
                    returnedString.Append((char)Marshal.ReadByte(new IntPtr((uint)pReturnedString + (uint)i)));
            }
            finally
            {
                Marshal.FreeCoTaskMem(pReturnedString);
            }

            string sectionData = returnedString.ToString();
            return sectionData.Split('\0');
        }

        /// <summary>
        /// Obtient le nom de toutes les sections.
        /// </summary>
        public string[] ReadSections()
        {
            const int bufferSize = 2048;

            StringBuilder returnedString = new StringBuilder();

            IntPtr pReturnedString = Marshal.AllocCoTaskMem(bufferSize);
            try
            {
                int bytesReturned = GetPrivateProfileSectionNames(pReturnedString, bufferSize, fileName);

                // bytesReturned -1 pour retirer le dernier \0
                for (int i = 0; i < bytesReturned - 1; i++)
                    returnedString.Append((char)Marshal.ReadByte(new IntPtr((uint)pReturnedString + (uint)i)));
            }
            finally
            {
                Marshal.FreeCoTaskMem(pReturnedString);
            }

            string sectionData = returnedString.ToString();
            return sectionData.Split('\0');
        }
    }
}
