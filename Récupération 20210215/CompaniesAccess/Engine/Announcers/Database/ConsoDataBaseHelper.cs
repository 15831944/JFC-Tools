using Database;
using System;
using System.Globalization;

namespace CompaniesAccess
{
    public class ConsoDataBaseHelper : IHelperDataBase
    {
        #region constructor

        public ConsoDataBaseHelper()
        {
            this.DataBaseInfos = new ConsoDatabaseInfos();
        }

        #endregion

        #region IHelperDataBase implementation

        /// <summary>
        /// the connection path
        /// </summary>
        public String PathConnection { get; set; }

        /// <summary>
        /// the connection username 
        /// </summary>
        public String UserNameConnection { get; set; }

        /// <summary>
        /// the connection password
        /// </summary>
        public String PasswordConnection { get; set; }

        /// <summary>
        /// the database informations
        /// </summary>
        public ADataBaseInfos DataBaseInfos { get; private set; }

        /// <summary>
        /// the special format
        /// </summary>
        public String DoubleFormat { get { return "G"; } }
        public IFormatProvider DoubleFormatProvider { get { return CultureInfo.InvariantCulture; } }

        #endregion
    }
}
