using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace JFCGridControl
{
    interface INotifyColumnMoved
    {
        event ColumnMovedEventHandler ColumnMoved;
    }

    public delegate void ColumnMovedEventHandler(object sender, ColumnMovedEventArgs e);

    public class ColumnMovedEventArgs
    {
        public virtual JFCGridColumn ColumnMoved { get; private set; }
        
        public ColumnMovedEventArgs(JFCGridColumn columnMoved)
        {
            ColumnMoved = columnMoved;
        }
    }
}
