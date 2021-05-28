using System;
using System.ComponentModel;
using System.ServiceProcess;

namespace JFCUpdateService
{
    //[RunInstaller(true)]
    //public class MyProjectInstaller : Installer
    //{
    //    private ServiceInstaller monServiceInstaller;

    //    private ServiceProcessInstaller monProcessInstaller;

    //    public MyProjectInstaller()
    //    {
    //        monProcessInstaller = new ServiceProcessInstaller();
    //        monServiceInstaller = new ServiceInstaller();
    //        monProcessInstaller.Account = ServiceAccount.LocalSystem;
    //        monServiceInstaller.StartType = ServiceStartMode.Manual;
    //        if (Environment.OSVersion.Version.Major <= 5)
    //        {
    //            string[] servicesDependedOn = new string[2] { "Netman", "Nla" };
    //            monServiceInstaller.ServicesDependedOn = servicesDependedOn;
    //        }
    //        monServiceInstaller.ServiceName = "JFC Update";
    //        monServiceInstaller.DisplayName = "JFC Update";
    //        monServiceInstaller.Description = "Téléchargement de mises à jour JFC.";
    //        base.Installers.Add(monServiceInstaller);
    //        base.Installers.Add(monProcessInstaller);
    //    }
    //}
}