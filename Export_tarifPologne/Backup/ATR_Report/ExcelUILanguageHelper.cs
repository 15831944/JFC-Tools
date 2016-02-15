using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;
using System.Threading;


namespace ATR_Report
{
    class ExcelUILanguageHelper : IDisposable
    {
        private CultureInfo m_CurrentCulture;

        public ExcelUILanguageHelper()
        {
            // save current culture and set culture to en-US
            m_CurrentCulture = Thread.CurrentThread.CurrentCulture;
            Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
        }

        #region IDisposable Members

        public void Dispose()
        {
            // return to normal culture
            Thread.CurrentThread.CurrentCulture = m_CurrentCulture;
        }

        #endregion
    } 
}
