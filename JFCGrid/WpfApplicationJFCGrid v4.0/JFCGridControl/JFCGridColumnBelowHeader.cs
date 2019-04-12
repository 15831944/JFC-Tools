using System.Windows.Controls;

namespace JFCGridControl
{
    public class JFCGridColumnBelowHeader : Control
    {
        public JFCGridColumnBelowHeader()
        {
            this.HeaderParent = null;
        }

        public JFCGridColumnBelowHeader(JFCGridColumnHeader parent)
        {
            this.HeaderParent = parent;
        }

        public JFCGridColumnHeader HeaderParent
        {
            get;
            private set;
        }

    }
}
