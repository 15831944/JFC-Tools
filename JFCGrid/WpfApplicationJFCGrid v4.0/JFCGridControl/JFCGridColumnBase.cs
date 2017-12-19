// JFCGridControl.JFCGridColumnBase
using JFCGridControl;
using System;
using System.ComponentModel;
using System.Linq.Expressions;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;


namespace JFCGridControl
{
    public class JFCGridColumnBase : DependencyObject, INotifyPropertyChanged
    {
        internal GridLength actualWidth;

        internal GridLength width;

        private uint widthAutoMax = 4294967295u;

        private string bindingPath;

        private Func<object, object> getterHandler;

        private Type getterRowType;

        public JFCGridCellStyle CellStyle = new JFCGridCellStyle();

        private ControlTemplate cellTemplate;

        private Thickness cellPadding = new Thickness(2.0, 1.0, 2.0, 1.0);

        public JFCGridColumn Parent;

        public virtual GridLength ActualWidth
        {
            get
            {
                return this.actualWidth;
            }
            set
            {
                GridLength gridLength = this.actualWidth;
                this.actualWidth = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("ActualWidth", gridLength, this.actualWidth));
            }
        }

        public virtual GridLength Width
        {
            get
            {
                return this.width;
            }
            set
            {
                GridLength gridLength = this.width;
                this.width = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("Width", gridLength, this.width));
                if (!this.width.IsAuto && !this.width.IsStar)
                {
                    this.ActualWidth = this.width;
                }
            }
        }

        public virtual uint WidthAutoMax
        {
            get
            {
                return this.widthAutoMax;
            }
            set
            {
                uint num = this.widthAutoMax;
                this.widthAutoMax = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("WidthAutoMax", num, this.widthAutoMax));
            }
        }

        public string BindingPath
        {
            get
            {
                return this.bindingPath;
            }
            set
            {
                if (value == null)
                {
                    throw new NullReferenceException("BindingPath");
                }
                this.getterHandler = null;
                this.getterRowType = null;
                string oldValue = this.bindingPath;
                this.bindingPath = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("BindingPath", oldValue, this.bindingPath));
            }
        }

        public ControlTemplate CellTemplate
        {
            get
            {
                return this.cellTemplate;
            }
            set
            {
                ControlTemplate oldValue = this.cellTemplate;
                this.cellTemplate = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("CellTemplate", oldValue, this.cellPadding));
            }
        }

        public Thickness CellPadding
        {
            get
            {
                return this.cellPadding;
            }
            set
            {
                Thickness thickness = this.cellPadding;
                this.cellPadding = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("CellPadding", thickness, this.cellPadding));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public JFCGridColumnBase()
        {
        }

        public JFCGridColumnBase(string BindingPath)
        {
            this.BindingPath = BindingPath;
        }

        public JFCGridColumnBase(Func<object, object> getterHandler)
        {
            this.getterHandler = getterHandler;
        }

        public bool GetCellValue(object row, out object value)
        {
            if (row == null)
            {
                value = null;
                return false;
            }
            Type type = row.GetType();
            if (type != this.getterRowType && this.BindingPath != null)
            {
                this.getterHandler = null;
                this.getterRowType = null;
            }
            if (this.getterHandler != null)
            {
                try
                {
                    value = this.getterHandler(row);
                    return true;
                }
                catch (Exception)
                {
                    if (this.BindingPath != null)
                    {
                        this.getterHandler = null;
                        this.getterRowType = null;
                    }
                }
            }
            if (this.BindingPath != null && this.BindingPath != string.Empty && type.GetProperty(this.BindingPath) != (PropertyInfo)null)
            {
                ParameterExpression parameterExpression = System.Linq.Expressions.Expression.Parameter(typeof(object), "rowInstance");
                UnaryExpression body = System.Linq.Expressions.Expression.Convert(System.Linq.Expressions.Expression.Property(System.Linq.Expressions.Expression.Convert(parameterExpression, type), this.BindingPath), typeof(object));
                this.getterHandler = System.Linq.Expressions.Expression.Lambda<Func<object, object>>((System.Linq.Expressions.Expression)body, new ParameterExpression[1]
                {
                parameterExpression
                }).Compile();
                this.getterRowType = type;
                value = this.getterHandler(row);
                return true;
            }
            value = null;
            return false;
        }

        internal void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, e);
            }
        }
    }
}