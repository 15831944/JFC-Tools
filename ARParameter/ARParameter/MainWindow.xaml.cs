using ARParameter.Module.File;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Configuration;

namespace ARParameter
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string AllConfigPath { get; set; }
        private string UserTableFolder { get; set; }

        private string ARTRADIOFolder = Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData) + @"\ARTRADIO";

        private ParametersUsersFile ParametersUsersFile;
        private UserFile UsFile;
        private ObservableCollection<ParametersUsersFile.UserParameters> UserParamsSelected = new ObservableCollection<ParametersUsersFile.UserParameters>();
        private ObservableCollection<string> Logs = new ObservableCollection<string>();

        #region Constructor
        public MainWindow()
        {
            InitializeComponent();

            ParamsIniLoading();

            ParamsFileLoading();

            ListViewParamsSelected.ItemsSource = UserParamsSelected;

            ListBoxLog.ItemsSource = Logs;
        }
        #endregion Constructor

        #region Event
        private void TextBoxEmail_KeyUp(object sender, KeyEventArgs e)
        {
            RefreshListViewFilterParams();
        }

        private void ButtonParamAdd_Click(object sender, RoutedEventArgs e)
        {
            if (((Button)e.Source).DataContext is ParametersUsersFile.UserParameters)
            {
                ParametersUsersFile.UserParameters uparams = (ParametersUsersFile.UserParameters)((Button)e.Source).DataContext;

                UserParamsSelected.Add(uparams);

                RefreshListViewFilterParams();
            }
        }

        private void ButtonParamDelete_Click(object sender, RoutedEventArgs e)
        {
            if (((Button)e.Source).DataContext is ParametersUsersFile.UserParameters)
            {
                ParametersUsersFile.UserParameters uparams = (ParametersUsersFile.UserParameters)((Button)e.Source).DataContext;

                UserParamsSelected.Remove(uparams);

                RefreshListViewFilterParams();
            }
        }

        private void ButtonValide_Click(object sender, RoutedEventArgs e)
        {
            bool isValid = true;

            // on teste si il y a au moins une selection
            if (UserParamsSelected.Count == 0)
            {
                MessageBox.Show("Il n'y a pas de configuration sélectionnée", "Erreur", MessageBoxButton.OK, MessageBoxImage.Error);
                isValid = false;
            }

            // on teste si la sélection est sur un seul utilisateur
            if (isValid && UserParamsSelected.Select(x => x.UserName).Distinct().Count() != 1)
            {
                MessageBox.Show("Les noms des utilisateurs sont différents dans la sélection", "Erreur", MessageBoxButton.OK, MessageBoxImage.Error);
                isValid = false;
            }

            // on teste si il n'y a qu'une seule compagnie de chaque
            if (isValid && UserParamsSelected.Select(x => x.Compagny).Distinct().Count() != UserParamsSelected.Count)
            {
                MessageBox.Show("Il y a plusieurs fois la même société dans la sélection", "Erreur", MessageBoxButton.OK, MessageBoxImage.Error);
                isValid = false;
            }

            if (isValid)
            {
                try
                {
                    this.Cursor = Cursors.Wait;

                    AddLog("Début de la configuration");

                    ParametersUsersFile.UserParameters userparam = UserParamsSelected.First();

                    if (CheckBoxTblOnly.IsChecked == false)
                    {
                        // Mise à jour des fichiers ini
                        // MAJ AU-Serv.ini
                        AUServConfigurator();

                        // MAJ ATAAETUD.INI
                        ATAAETUDConfigurator(userparam);

                        // MAJ UFRXX.INI
                        UFRXXConfigurator(userparam);

                        // mise à jour de la base de registre
                        RegistryUpdate(userparam);
                    }

                    // mise à jour du fichier utilisateur
                    UpdateUserFile();

                    AddLog("Fin de la configuration");

                    MessageBox.Show("La configuration du poste est terminée", "Terminé", MessageBoxButton.OK,
                        MessageBoxImage.None);
                }
                finally
                {
                    this.Cursor = Cursors.Arrow;
                }
            }

        }

        private void ButtonCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        #endregion Event

        #region Method

        private void ParamsIniLoading()
        {
            AddLog("Start loading : ARParameter.ini");



            IniFile ini = new IniFile(Environment.CurrentDirectory + @"\ARParameter.ini");
            AllConfigPath = ini.ReadString("Parameter", "AllConfigPath");
            UserTableFolder = ini.ReadString("Parameter", "UserTableFolder");

            AddLog("File load : ARParameter.ini");

            if (string.IsNullOrEmpty(AllConfigPath))
            {
                AddLog("AllConfigPath n'est pas correctement renseigné dans ARParameter.ini");

                MessageBox.Show("AllConfigPath n'est pas correctement renseigné dans ARParameter.ini", "Erreur", MessageBoxButton.OK,
                        MessageBoxImage.Error);
            }

            if (string.IsNullOrEmpty(UserTableFolder))
            {
                AddLog("UserTableFolder n'est pas correctement renseigné dans ARParameter.ini");

                MessageBox.Show("UserTableFolder n'est pas correctement renseigné dans ARParameter.ini", "Erreur", MessageBoxButton.OK,
                        MessageBoxImage.Error);
            }
        }

        private void ParamsFileLoading()
        {
            if (System.IO.File.Exists(AllConfigPath))
            {
                AddLog("Start loading : " + AllConfigPath);

                ParametersUsersFile = new ParametersUsersFile(AllConfigPath);
                ParametersUsersFile.ReadFile();

                AddLog("File load : " + AllConfigPath);
            }
            else
            {
                MessageBox.Show("le fichier " + AllConfigPath + " n'existe pas", "Erreur", MessageBoxButton.OK,
                        MessageBoxImage.Error);
                AddLog("This file doesn't exist : " + AllConfigPath);
            }
        }

        private void RefreshListViewFilterParams()
        {
            string email = TextBoxEmail.Text;

            ListViewFilterParams.ItemsSource = ParametersUsersFile.UserParams.Select(x => x).Where(d => d.Email.ToLower().Contains(email.ToLower()) && !UserParamsSelected.Contains(d));
        }

        private void AUServConfigurator()
        {
            AddLog("Début des modification dans AU-Serv.ini");

            IniFile auServFile = new IniFile(ARTRADIOFolder + @"\AU-Serv.ini");

            // delete "ExcuteBefore" information
            auServFile.WriteString("Parametres", "ExecuteBefore", "");

            if (UserParamsSelected.Count > 1 || !string.IsNullOrEmpty(UserParamsSelected.First().UserTablePath) || UserParamsSelected.First().ExecutableToLaunch.ToLower() == "multiuseredi.exe")
                auServFile.WriteString("Parametres", "Execute", "MultiUserEDI.exe");
            else if (UserParamsSelected.Count == 1)
                auServFile.WriteString("Parametres", "Execute", UserParamsSelected.First().ExecutableToLaunch);
            else if (UserParamsSelected.Count == 1)
                auServFile.WriteString("Parametres", "Execute", "atsauve.exe");


            auServFile.WriteString("Parametres", "RemoteDir", UserParamsSelected.First().UpdatePath);

            if (RadioButtonRDS.IsChecked == true)
            {
                // RDS mode
                auServFile.WriteString("Parametres", "Company", "Havas Media Virtuel");
                auServFile.WriteString("Parametres", "SerialVirtual", "A2B838EE002564C2C365");
                auServFile.WriteString("Parametres", "ProductCode", "GD5JC6GNBB2SBQZ9W9Y2JBA6I");
                auServFile.WriteString("Parametres", "Cle", "F6E6266F");
            }
            else
            {
                // Local mode
                auServFile.WriteString("Parametres", "Company", "Havas Media");
                auServFile.WriteString("Parametres", "SerialGroup", "A2B838EE002564C2C365");
                auServFile.WriteString("Parametres", "ProductCode", "2IWEIFNP0FGMTPAKXCPLEZ665");
                auServFile.WriteString("Parametres", "Cle", "7DAD4A19");
            }

            AddLog("Fin des modification dans AU-Serv.ini");
        }

        private void ATAAETUDConfigurator(ParametersUsersFile.UserParameters userparam)
        {
            AddLog("Début des modification dans ATAAETUD.ini");

            IniFile auServFile = new IniFile(ARTRADIOFolder + @"\ATAAETUD.ini");

            auServFile.WriteString("Definition_du_site", "CheminReseau", userparam.NetworkMode ? "1" : "0");

            if (UserParamsSelected.Count > 1 || !string.IsNullOrEmpty(userparam.UserTablePath) || userparam.ExecutableToLaunch.ToLower() == "multiuseredi.exe")
            {
                string userTableFile = UserTableFolder + @"\TableUtilisateur_" + userparam.UserName + ".tbl";

                if (!string.IsNullOrEmpty(userparam.UserTablePath)) userTableFile = userparam.UserTablePath;

                auServFile.WriteString("Localisation", "USER_TABLE", userTableFile);
            }

            auServFile.WriteString("DefOption", "Nomenclature", userparam.NomenclaturePath);

            auServFile.WriteString("Localisation", "EXTERNE", userparam.EDIPath);

            auServFile.WriteString("Localisation", "N_CLIENT", userparam.NomenclaturePath2);

            auServFile.WriteString("Inter", "NomEmetteur", userparam.UserName);

            auServFile.WriteString("Localisation", "CORRESPONDANTS", userparam.UserInternTablePath);

            if (RadioButtonRDS.IsChecked == true)
                auServFile.WriteString("DefOption", "E_Pige", "");
            else
                auServFile.WriteString("DefOption", "E_Pige", "1");

            AddLog("Fin des modification dans ATAAETUD.ini");
        }

        private void UFRXXConfigurator(ParametersUsersFile.UserParameters userparam)
        {
            foreach (var file in System.IO.Directory.GetFiles(ARTRADIOFolder, "UFR*.ini"))
            {
                AddLog("Début des modification dans " + file);

                IniFile auServFile = new IniFile(file);

                auServFile.WriteString("SAS", "Repertoire SE", userparam.EDIPath);

                auServFile.WriteString("SAS", "Repertoire SI", userparam.UserPath);

                AddLog("Fin des modification dans " + file);
            }
        }

        private void RegistryUpdate(ParametersUsersFile.UserParameters userparam)
        {
            AddLog("Début des modification dans la base de registre");

            const string userRoot = "HKEY_CURRENT_USER";

            const string keyName = userRoot + @"\Software\JFC\Nomenclature\Configuration";

            Registry.SetValue(keyName, "DataAcces", "2");
            Registry.SetValue(keyName, "EDIAcces", "2");
            Registry.SetValue(keyName, "DataPath", userparam.NomenclaturePath);
            Registry.SetValue(keyName, "UserName", userparam.UserName);
            Registry.SetValue(keyName, "NomenclPath", ARTRADIOFolder);

            AddLog("Fin des modification dans la base de registre");
        }

        private void UpdateUserFile()
        {
            if (UserParamsSelected.Count > 1 || !string.IsNullOrEmpty(UserParamsSelected.First().UserTablePath) || UserParamsSelected.First().ExecutableToLaunch.ToLower() == "multiuseredi.exe")
            {
                string userTableFile = UserTableFolder + @"\TableUtilisateur_" + UserParamsSelected.First().UserName + ".tbl";

                AddLog("Début de l'écriture du fichier " + userTableFile);

                UsFile = new UserFile(userTableFile);

                // parametrage du fichier utilisateur
                UserFile.Compagny cp;
                foreach (var item in UserParamsSelected)
                {
                    cp = new UserFile.Compagny();
                    cp.Name = item.Compagny;
                    cp.UserPath = item.UserPath;
                    cp.EDIPath = item.EDIPath;
                    cp.ComptoireCode = item.EDICode;
                    cp.NomenclaturePath = item.NomenclaturePath;
                    cp.UserEmail = item.Email;

                    if (!string.IsNullOrEmpty(item.ext_raison))
                        cp.RaisonFile = "RAISON." + item.ext_raison;
                    else
                        cp.RaisonFile = "RAISON.NEW";

                    UsFile.Compagnies.Add(cp);
                }

                // Création du fichier utilisateur_XXX.tbl
                UsFile.SaveFile();

                AddLog("Fin de l'écriture du fichier " + userTableFile);
            }
        }

        private void AddLog(string message)
        {
            Logs.Add(message);
        }
        #endregion Method

    }
}
