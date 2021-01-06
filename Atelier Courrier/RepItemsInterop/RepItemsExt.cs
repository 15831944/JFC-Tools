using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace RepItemsIterop
{
    public class RepItemsExt
    {

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern void SetupIniFile([MarshalAs(UnmanagedType.AnsiBStr)]string iniFile);

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern IntPtr GetSourcesPath();

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern IntPtr GetConfiguration([MarshalAs(UnmanagedType.AnsiBStr)]string keyVal);

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern IntPtr GetIniValue(
            [MarshalAs(UnmanagedType.AnsiBStr)]string sectionVal,
            [MarshalAs(UnmanagedType.AnsiBStr)]string keyVal);

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern void ShowRepItem(int lang, int idSrc, int idTerr, int idPoids);

        //[DllImport("RepItemsWrap.dll")]
        //internal static extern IntPtr TestString();

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern bool LoadTerrain(int idSrc, int idTerr, int idPoids);

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern int GetIndivCount();

        //[DllImport("RepItemsWrap.dll")]
        //internal static extern unsafe int GetPoids(double* buffer, int size);

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern int GetPoids(IntPtr buffer, int size);

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern int GetPoids([MarshalAs(UnmanagedType.LPArray)]double[] buffer, int size);

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern void MoveFirstItem();

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern bool IsValidItem();

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern void MoveNextItem();

        [StructLayout(LayoutKind.Sequential)]
        public struct Item
        {
            public int id;

            [MarshalAs(UnmanagedType.LPStr)]
            public string label; 
        };

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern void GetItem(ref Item item);

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern int GetItemIndivCount(int itemId);

        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern int GetItemIndivVector(int itemId, IntPtr buffer, int size);

        
        [DllImport(@"Libs\RepItemsWrap.dll")]
        internal static extern int GetItemIndivVector(int itemId, [MarshalAs(UnmanagedType.LPArray)]int[] buffer, int size);

        [STAThread]
        static void Main()
        {

            SetupIniFile(@"C:\Temp\test.ini");

            IntPtr ptr = GetSourcesPath();
            string tmp = Marshal.PtrToStringAnsi(ptr);

            string test1 = RepItemsBuddy.GetInstallDir();
            string test2 = RepItemsBuddy.GetUserPath();
            
            ShowRepItem(0, 37, 338, 44);


            if (LoadTerrain(37, 338, 44))
            {
                int size = GetIndivCount();
                int poidsCount = 0;

                double[] dVector = new double[size];
                poidsCount = GetPoids(dVector, size);

                //double[] dArray = new double[size];

                //unsafe
                //{
                //    fixed (double* pDBuffer = dArray)
                //    {
                //        poidsCount = GetPoids(pDBuffer, size);
                //    }
                //}

                double[] dArray2 = new double[size];
                IntPtr dArray2Buffer = System.Runtime.InteropServices.Marshal.UnsafeAddrOfPinnedArrayElement(dArray2, 0);
                poidsCount = GetPoids(dArray2Buffer, size);

                IntPtr dBuffer = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(double)) * size);

                poidsCount = GetPoids(dBuffer, size);

                double[] poids = new double[poidsCount];

                Marshal.Copy(dBuffer, poids, 0, poidsCount);
                Marshal.FreeHGlobal(dBuffer);

                Item item = new Item();
                for (MoveFirstItem(); IsValidItem(); MoveNextItem())
                {
                    GetItem(ref item);

                    int indivCount = size;
                    IntPtr iBuffer = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(int)) * indivCount);
                    indivCount = GetItemIndivVector(item.id, iBuffer, indivCount);

                    int[] itemVector = new int[indivCount];
                    Marshal.Copy(iBuffer, itemVector, 0, indivCount);

                }

            }

            //string str = Marshal.PtrToStringAnsi(TestString());
            
            
        }
    }

}
