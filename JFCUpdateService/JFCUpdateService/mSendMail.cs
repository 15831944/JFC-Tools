using System;
using System.Net.Mail;
using JFCUpdateService;
using Microsoft.VisualBasic.CompilerServices;


namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mSendMail
    {
        public static void SendMail(string Message, string subject = null)
        {
            try
            {
                SmtpClient smtpClient = new SmtpClient();
                string NomModule = "Mail";
                string MotCle = "SmtpServer";
                smtpClient.Host = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref MonService.svServiceIni);
                if (Operators.CompareString(smtpClient.Host, "", TextCompare: false) != 0)
                {
                    MotCle = "Mail";
                    NomModule = "From";
                    string from = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref MonService.svServiceIni);
                    NomModule = "Mail";
                    MotCle = "Recipient";
                    string recipients = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref MonService.svServiceIni);
                    subject = ((Operators.CompareString(subject, null, TextCompare: false) == 0) ? MonService.DisplayNameService : (MonService.DisplayNameService + " - " + subject));
                    smtpClient.Send(from, recipients, subject, Message);
                }
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