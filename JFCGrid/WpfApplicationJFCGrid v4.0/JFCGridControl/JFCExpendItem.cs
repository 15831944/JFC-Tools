// JFCGridControl.JFCExpendItem
using JFCGridControl;
using System.Collections.Generic;
using System.Windows;


namespace JFCGridControl
{
    internal class JFCExpendItem : DependencyObject, INotifyPropertyChangedExtended<object>
    {
        private short level;

        private int startChildViewIndex = -1;

        private int nbChildrenView;

        private object obj;

        public JFCExpendItem Parent;

        public List<JFCExpendItem> Children = new List<JFCExpendItem>();

        private bool isExpendable;

        public static readonly DependencyProperty IsExpendedProperty = DependencyProperty.Register("IsExpended", typeof(bool), typeof(JFCExpendItem), new UIPropertyMetadata(false, JFCExpendItem.UpdateIsExpendedProperty));

        public short Level
        {
            get
            {
                return this.level;
            }
            set
            {
                short num = this.level;
                this.level = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("Level", num, value));
            }
        }

        public int StartChildViewIndex
        {
            get
            {
                return this.startChildViewIndex;
            }
            set
            {
                int num = this.startChildViewIndex;
                this.startChildViewIndex = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("StartChildViewIndex", num, value));
            }
        }

        public int NbChildrenView
        {
            get
            {
                return this.nbChildrenView;
            }
            set
            {
                int num = this.nbChildrenView;
                this.nbChildrenView = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("NbChildrenView", num, value));
            }
        }

        public object Obj
        {
            get
            {
                return this.obj;
            }
            set
            {
                object oldValue = this.obj;
                this.obj = value;
                this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("Obj", oldValue, value));
            }
        }

        public bool IsExpendable
        {
            get
            {
                return this.isExpendable;
            }
            set
            {
                if (this.IsExpendable != value)
                {
                    bool flag = this.isExpendable;
                    this.isExpendable = value;
                    this.OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("IsExpendable", flag, value));
                }
            }
        }

        public bool IsExpended
        {
            get
            {
                return (bool)base.GetValue(JFCExpendItem.IsExpendedProperty);
            }
            set
            {
                if (this.IsExpended != value)
                {
                    base.SetValue(JFCExpendItem.IsExpendedProperty, value);
                }
            }
        }

        public event PropertyChangedExtendedEventHandler<object> PropertyChanged;

        public JFCExpendItem()
        {
        }

        public JFCExpendItem(object obj)
        {
            this.obj = obj;
        }

        private static void UpdateIsExpendedProperty(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            if (e.NewValue != e.OldValue)
            {
                (obj as JFCExpendItem).OnPropertyChanged(new PropertyChangedExtendedEventArgs<object>("IsExpended", e.OldValue, e.NewValue));
            }
        }

        internal void OnPropertyChanged(PropertyChangedExtendedEventArgs<object> e)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, e);
            }
        }

        public void ResetEvent()
        {
            this.PropertyChanged = null;
        }
    }
}