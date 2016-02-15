using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace TarifsPresse.Destination.Classes
{
    public interface IData
    {
        bool read(FileInfo file, ProgressBar progressCtrl, DateTime _date);
    }
}
