using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Controls;
using System.Windows.Data;


namespace JFCGridControl
{
    //public class JFCGridColumnBase : System.Windows.DependencyObject, INotifyPropertyChangedExtended<Object>
    public class JFCGridColumnBase : System.Windows.DependencyObject, INotifyPropertyChanged
    {
        public JFCGridColumnBase()
        {
            //Header = new JFCGridColumnHeader();

            //CreateDefaultStyle();
        }

        public JFCGridColumnBase(string BindingPath)
        {
            this.BindingPath = BindingPath;
            //Header = new JFCGridColumnHeader();

            //CreateDefaultStyle();
        }

        public JFCGridColumnBase(Func<object, object> getterHandler)
        {
            this.getterHandler = getterHandler;
        }

        //private void CreateDefaultStyle()
        //{
        //    style = new Style();

        //    ControlTemplate tmp = new ControlTemplate();

        //    FrameworkElementFactory textblock = new
        //    FrameworkElementFactory(typeof(TextBlock));


        //    tmp.VisualTree = new FrameworkElementFactory(typeof(Grid));
        //    tmp.VisualTree.AppendChild(textblock);

        //    Binding b = new Binding();
        //    textblock.SetBinding(TextBlock.TextProperty, b);

        //    Setter setter = new Setter(ContentControl.TemplateProperty, tmp);

        //    style.Setters.Add(setter);
        //}

        //private Visibility visible = Visibility.Visible;
        //public Visibility Visible
        //{
        //    get { return visible; }
        //    set
        //    {
        //        visible = value;
        //        OnPropertyChanged(new PropertyChangedEventArgs("Visible"));
        //    }
        //}

        //private Boolean frozen = false;
        //public Boolean Frozen
        //{
        //    get { return frozen; }
        //    set
        //    {
        //        frozen = value;
        //        OnPropertyChanged(new PropertyChangedEventArgs("Frozen"));
        //    }
        //}

        internal GridLength actualWidth = new GridLength();
        public virtual GridLength ActualWidth
        {
            get
            {
                return actualWidth;
            }
            set
            {
                var oldvalue = actualWidth;
                actualWidth = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("ActualWidth", oldvalue, actualWidth));
            }
        }

        internal GridLength width = new GridLength();
        public virtual GridLength Width
        {
            get
            {
                return width;
            }
            set
            {
                var oldvalue = width;
                width = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Width", oldvalue, width));

                if (!width.IsAuto && !width.IsStar)
                    ActualWidth = width;
            }
        }

        private UInt32 widthAutoMax = UInt32.MaxValue;
        public virtual UInt32 WidthAutoMax
        {
            get
            {
                return widthAutoMax;
            }
            set
            {
                var oldvalue = widthAutoMax;
                widthAutoMax = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("WidthAutoMax", oldvalue, widthAutoMax));
            }
        }

        private string bindingPath;
        public string BindingPath
        {
            get { return bindingPath; }
            set
            {
                if (value == null)
                    throw new NullReferenceException("BindingPath");

                getterHandler = null;
                getterRowType = null;

                var oldvalue = bindingPath;
                bindingPath = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("BindingPath", oldvalue, bindingPath));
            }
        }


        private Func<object, object> getterHandler;
        private Type getterRowType;
        public bool GetCellValue(object row, out object value)
        {
            if (row == null)
            {
                value = null;
                return false;
            }

            Type rowType = row.GetType();
            if (rowType != getterRowType)
            {
                if (BindingPath != null)
                {
                    getterHandler = null;
                    getterRowType = null;
                }
            }

            if (getterHandler != null)
            {
                try
                {
                    value = getterHandler(row);
                    return true;
                }
                catch (Exception)
                {
                    if (BindingPath != null)
                    {
                        getterHandler = null;
                        getterRowType = null;
                    }
                }
            }

            if (BindingPath != null && BindingPath != string.Empty && rowType.GetProperty(BindingPath) != null)
            {

                // Creating a parameter expression.
                var rowInstance = System.Linq.Expressions.Expression.Parameter(typeof(object), "rowInstance");
                var gblock = System.Linq.Expressions.Expression.Convert(
                    System.Linq.Expressions.Expression.Property(System.Linq.Expressions.Expression.Convert(rowInstance, rowType), BindingPath),
                    typeof(object));

                // Compile and execute an expression tree.
                getterHandler = System.Linq.Expressions.Expression.Lambda<Func<object, object>>(gblock, rowInstance).Compile();
                getterRowType = rowType;

                value = getterHandler(row);
                return true;
            }
            value = null;
            return false;
        }

        public JFCGridCellStyle CellStyle = new JFCGridCellStyle();

        private ControlTemplate cellTemplate = null;
        public ControlTemplate CellTemplate
        {
            get { return cellTemplate; }
            set
            {
                var oldvalue = cellTemplate;
                cellTemplate = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("CellTemplate", oldvalue, cellPadding));
            }
        }

        //private Style style;
        //public Style Style
        //{
        //    get { return style; }
        //    set
        //    {
        //        style = value;
        //        OnPropertyChanged(new PropertyChangedEventArgs("Style"));
        //    }
        //}

        //
        private Thickness cellPadding = new Thickness(2, 1, 2, 1);
        public Thickness CellPadding
        {
            get { return cellPadding; }
            set
            {
                var oldvalue = cellPadding;
                cellPadding = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("CellPadding", oldvalue, cellPadding));
            }
        }

        ////
        //public event PropertyChangedExtendedEventHandler<Object> PropertyChanged;
        //internal void OnPropertyChanged(PropertyChangedExtendedEventArgs<Object> e)
        //{
        //    if (PropertyChanged != null)
        //        PropertyChanged(this, e);
        //}

        public event PropertyChangedEventHandler PropertyChanged;
        internal void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, e);
        }

        //
        public JFCGridColumn Parent = null;

    }
}
