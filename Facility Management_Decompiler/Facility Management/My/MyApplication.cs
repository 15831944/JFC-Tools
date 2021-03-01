﻿// Facility_Management.My.MyApplication
using System;
using System.CodeDom.Compiler;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Windows.Forms;
using Facility_Management.My;
using Microsoft.VisualBasic.ApplicationServices;

namespace Facility_Management.My
{
    [GeneratedCode("MyTemplate", "11.0.0.0")]
    [EditorBrowsable(EditorBrowsableState.Never)]
    internal class MyApplication : WindowsFormsApplicationBase
    {
        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        [STAThread]
        //[DebuggerHidden]
        [EditorBrowsable(EditorBrowsableState.Advanced)]
        internal static void Main(string[] Args)
        {
            Application.SetCompatibleTextRenderingDefault(WindowsFormsApplicationBase.UseCompatibleTextRendering);
            MyProject.Application.Run(Args);
        }

        //[DebuggerStepThrough]
        public MyApplication()
            : base(AuthenticationMode.Windows)
        {
            base.IsSingleInstance = false;
            base.EnableVisualStyles = true;
            base.SaveMySettingsOnExit = true;
            base.ShutdownStyle = ShutdownMode.AfterMainFormCloses;
        }

        //[DebuggerStepThrough]
        protected override void OnCreateMainForm()
        {
            base.MainForm = MyProject.Forms.FormBox;
        }
    }
}
