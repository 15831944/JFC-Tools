using System;
using RadioEngine;
using System.ComponentModel;

namespace CompaniesAccess
{
    public enum RightsMode
    {
        [Description("Droits d'accès sur les sociétés")]
        Societes,
        [Description("Droits d'accès sur les annonceurs")]
        Annonceurs
    };

    public static class UserDataManagerFactory
    {
        public static IUserDataManager Provide(RightsMode _mode)
        {
            IUserDataManager l_manager = null;

            switch (_mode)
            {
                case RightsMode.Societes:
                    l_manager = new CompaniesManager();
                    break;
                case RightsMode.Annonceurs:
                    l_manager = new AnnouncersManager();
                    break;
            }

            return l_manager;
        }
    }
}
