using Facility_Management.My;
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace Facility_Management
{
    internal sealed class mCopyFile
    {
        private delegate void CopyProgressRoutineCallback(long TotalFileSize, long TotalBytesTransferred, long StreamSize, long StreamBytesTransferred, int dwStreamNumber, int dwCallbackReason, int hSourceFile, int hDestinationFile, int lpData);

        private const short PROGRESS_CANCEL = 1;

        private const short PROGRESS_CONTINUE = 0;

        private const short PROGRESS_QUIET = 3;

        private const short PROGRESS_STOP = 2;

        public const short PROGRESS_FORM_CREATEUPDATE = 1;

        public const short PROGRESS_FORM_FORMBOX = 2;

        private const int COPY_FILE_FAIL_IF_EXISTS = 1;

        private const int COPY_FILE_RESTARTABLE = 2;

        public static int bCancel;

        private static bool m_bProgress;

        private static short m_Percent;

        private static short m_ProgressForm;

        [DllImport("kernel32", CharSet = CharSet.Ansi, EntryPoint = "CopyFileA", ExactSpelling = true, SetLastError = true)]
        private static extern int apiCopyFile([MarshalAs(UnmanagedType.VBByRefStr)] ref string lpExistingFileName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpNewFileName, int bFailIfExists);

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, EntryPoint = "CopyFileExA", ExactSpelling = true, SetLastError = true)]
        private static extern int CopyFileEx([MarshalAs(UnmanagedType.VBByRefStr)] ref string lpExistingFileName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpNewFileName, CopyProgressRoutineCallback lpProgressRoutine, ref object lpData, ref int pbCancel, int dwCopyFlags);

        public static bool CopyFileWithProgress(ref string Source, ref string Destination, short ProgressForm)
        {
            int num = 0;
            bool flag = true;
            m_ProgressForm = ProgressForm;
            if (!flag)
            {
                num = 1;
            }
            if (true)
            {
                num |= 2;
            }
            m_bProgress = false;
            m_Percent = 0;
            MyProject.Forms.FormBox.ToolStripProgressBar1.Maximum = 100;
            CopyProgressRoutineCallback lpProgressRoutine = delegate (long a0, long a1, long a2, long a3, int a4, int a5, int a6, int a7, int a8)
            {
                CopyProgressRoutine(a0, a1, a2, a3, a4, a5, a6, a7, a8);
            };
            object lpData = 0;
            return CopyFileEx(ref Source, ref Destination, lpProgressRoutine, ref lpData, ref bCancel, num) != 0;
        }

        private static int CopyProgressRoutine(long TotalFileSize, long TotalBytesTransferred, long StreamSize, long StreamBytesTransferred, int dwStreamNumber, int dwCallbackReason, int hSourceFile, int hDestinationFile, int lpData)
        {
            if (TotalFileSize > 0)
            {
                short num = checked((short)Conversion.Int((double)(TotalBytesTransferred * 10000) / (double)(TotalFileSize * 10000) * 100.0));
                Application.DoEvents();
                if (num > m_Percent)
                {
                    if (m_ProgressForm == 1)
                    {
                        MyProject.Forms.CreateUpdate.ToolStripProgressBar1.Value = m_Percent;
                    }
                    else if (m_ProgressForm == 2)
                    {
                        MyProject.Forms.FormBox.ToolStripProgressBar1.Value = m_Percent;
                    }
                    m_Percent = num;
                }
            }
            return 0;
        }

        public static bool CopyFile(string SourceFileName, string TargetFileName)
        {
            //Discarded unreachable code: IL_00a6, IL_00e8, IL_00ea, IL_00fa, IL_011c
            int num2 = default(int);
            bool result = false;
            int num4 = default(int);
            try
            {
                int num = 1;
                string text = "";
                num = 2;
                string text2 = "";
                num = 3;
                bool flag = false;
                num = 4;
                checked
                {
                    text = SourceFileName + Strings.Space(250 - Strings.Len(SourceFileName));
                    num = 5;
                    text2 = TargetFileName + Strings.Space(250 - Strings.Len(TargetFileName));
                    num = 6;
                }
                int bFailIfExists = 0 - (flag ? 1 : 0);
                ProjectData.ClearProjectError();
                num2 = 1;
                num = 8;
                int num3 = apiCopyFile(ref text, ref text2, bFailIfExists);
                ProjectData.ClearProjectError();
                num2 = 0;
                num = 10;
                if (Information.Err().Number != 0)
                {
                    num = 11;
                    Interaction.MsgBox(Information.Err().Description);
                }
                num = 12;
                result = num3 != 0;
            }
            catch (Exception obj) when (obj is Exception && num2 != 0 && num4 == 0)
            {
                ProjectData.SetProjectError((Exception)obj);
                /*Error near IL_011a: Could not find block for branch target IL_00ea*/
                ;
            }
            if (num4 != 0)
            {
                ProjectData.ClearProjectError();
            }
            return result;
        }
    }

}
