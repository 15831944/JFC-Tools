using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace RepItemsIterop
{
    public static class RepItemsBuddy
    {
        private static string iniFile = null;

        public static void SetupIniFile(string iniFile)
        {
            RepItemsBuddy.iniFile = iniFile;
            RepItemsExt.SetupIniFile(iniFile);
        }

        #region Configuration...

        public static string GetConfiguration(string keyValue)
        {
            IntPtr ptr = RepItemsExt.GetConfiguration(keyValue);
            return Marshal.PtrToStringAnsi(ptr);
        }

        public static string GetCheminData()
        {
            return GetConfiguration("CheminData");
        }

        public static string GetCheminDataItem()
        {
            return GetConfiguration("CheminDataItem");
        }

        public static string GetFivia()
        {
            return GetConfiguration("Fivia");
        }

        public static string GetInstallDir()
        {
            return GetConfiguration("InstallDir");
        }

        public static string GetSeuilIndivCible()
        {
            return GetConfiguration("SeuilIndivCible");
        }

        public static string GetSeuilMapping()
        {
            return GetConfiguration("SeuilMapping");
        }

        public static string GetSeuilSite1()
        {
            return GetConfiguration("SeuilSite1");
        }

        public static string GetSeuilSite2()
        {
            return GetConfiguration("SeuilSite2");
        }

        public static string GetTitreAppli()
        {
            return GetConfiguration("TitreAppli");
        }

        public static string GetUserPath()
        {
            IntPtr ptr = RepItemsExt.GetIniValue("Data", "UserPath");
            return Marshal.PtrToStringAnsi(ptr);
        }

        public static string GetLang()
        {
            return GetConfiguration("Lang");
        }

        public static int GetLangId()
        {
            string lang = GetLang().Trim().ToLower();
            if (lang == "fr")
            {
                return 1;
            }
            else if (lang == "en")
            {
                return 0;
            }
            else if (lang == "it")
            {
                return 2;
            }
            return -1;
        }

        #endregion

        public static bool LoadTerrain(int idSrc, int idTerr, int idPoids)
        {
            return RepItemsExt.LoadTerrain(idSrc, idTerr, idPoids);
        }

        public static int GetIndivCount()
        {
            return RepItemsExt.GetIndivCount();
        }

        public static void ShowRepItem(int idSrc, int idTerr, int idPoids)
        {
            RepItemsExt.ShowRepItem(GetLangId(), idSrc, idTerr, idPoids);
        }

        public static double[] GetPoids()
        {
            int size = GetIndivCount();
            double[] dVector = new double[size];
            RepItemsExt.GetPoids(dVector, size);
            return dVector;
        }

        public static void MoveFirstItem()
        {
            RepItemsExt.MoveFirstItem();
        }

        public static bool IsValidItem()
        {
            return RepItemsExt.IsValidItem();
        }

        public static void MoveNextItem()
        {
            RepItemsExt.MoveNextItem();
        }

        public static RepItemsExt.Item GetItem()
        {
            RepItemsExt.Item item = new RepItemsExt.Item();
            RepItemsExt.GetItem(ref item);
            return item;
        }

        public static void GetItemIndivVector(RepItemsExt.Item item, int[] indivVector)
        {
            if (indivVector.Length != GetIndivCount())
                throw new Exception("The given array length must by " + GetIndivCount());
            RepItemsExt.GetItemIndivVector(item.id, indivVector, indivVector.Length);
        }
    }
}
