using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace AtOracle_NRJ
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {

            //new ConnecOracle("3;NR;NA;01/02/2010;14/03/2010;\\\\jfc-data\\Utilisateur\\transfert\\liem\\Dispo_NRJ\\NRJ_3912_NR_1005041050");


            //new ConnecOracle("3;RI;NA;22/03/2010;02/05/2010;C:\\3899_RI_1003291637");


            //new ConnecOracle("5;\\\\jfc-data\\Utilisateur\\transfert\\Stephane\\Oracle\\NRJ_3934_CH1_ECR1005191844");
            //new ConnecOracle("5;\\\\jfc-data\\Utilisateur\\transfert\\Stephane\\Oracle\\NRJ_3916_MF_ECR1004291316");

            new ConnecOracle("3;CH;NA;22/03/2010;02/05/2010;NRJ_TEST_22042010_3espace;;4902 490201 490203");
            //new ConnecOracle("5;PlanExp1");
           // new WarningForm();
           
            
           //string chaineArg = args[0].ToString();
           //new ConnecOracle(chaineArg);

           // new ConnecOracle(new WarningForm("Veuillez patienter"), "3;RI;22/03/2010;02/05/2010;C:\\3899_RI_1003291637");
           //new WarningForm("veillez patienter", "3;RI;22/03/2010;02/05/2010;C:\\3899_RI_1003291637");

        }

 
    }
}
