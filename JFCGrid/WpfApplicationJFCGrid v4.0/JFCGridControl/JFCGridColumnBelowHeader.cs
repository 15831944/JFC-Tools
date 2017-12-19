using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;

namespace JFCGridControl
{
    public class JFCGridColumnBelowHeader : Control
    {
        public JFCGridColumnBelowHeader()
        {
            this.Parent = null;
        }

        public JFCGridColumnBelowHeader(JFCGridColumnHeader Parent)
        {
            this.Parent = Parent;
        }

        public JFCGridColumnHeader Parent
        {
            get;
            private set;
        }

    }
}
