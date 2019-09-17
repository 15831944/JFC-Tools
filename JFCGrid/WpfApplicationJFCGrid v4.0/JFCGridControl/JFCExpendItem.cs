using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows;

namespace JFCGridControl
{
    public class JFCExpendItem : DependencyObject, INotifyPropertyChangedExtended<object>
    {
        public JFCExpendItem()
        { }

        public JFCExpendItem(Object obj)
        {
            this.obj = obj;
        }

        Int16 level = 0;
        public Int16 Level
        {
            get { return level; }
            set
            {
                Int16 oldValue = level;
                level = value;
                //OnPropertyChanged(new PropertyChangedEventArgs("Level"));
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Level", oldValue, value));
            }
        }

        Int32 startChildViewIndex = -1;
        public Int32 StartChildViewIndex
        {
            get { return startChildViewIndex; }
            set
            {
                Int32 oldValue = startChildViewIndex;
                startChildViewIndex = value;
                //OnPropertyChanged(new PropertyChangedEventArgs("StartChildIndex"));
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("StartChildViewIndex", oldValue, value));
            }
        }

        Int32 nbChildrenView = 0;
        public Int32 NbChildrenView
        {
            get { return nbChildrenView; }
            set
            {
                Int32 oldValue = nbChildrenView;
                nbChildrenView = value;
                //OnPropertyChanged(new PropertyChangedEventArgs("NbChildren"));
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("NbChildrenView", oldValue, value));
            }
        }

        Object obj = null;
        public Object Obj
        {
            get { return obj; }
            set
            {
                Object oldValue = obj;
                obj = value;
                //OnPropertyChanged(new PropertyChangedEventArgs("Obj"));
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Obj", oldValue, value));
            }
        }

        public JFCExpendItem Parent = null;
        public List<JFCExpendItem> Children = new List<JFCExpendItem>();

        Boolean isExpendable = false;
        public Boolean IsExpendable
        {
            get { return isExpendable; }
            set
            {
                if (IsExpendable != value)
                {
                    Boolean oldValue = isExpendable;
                    isExpendable = value;
                    //OnPropertyChanged(new PropertyChangedEventArgs("IsExpendable"));
                    OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("IsExpendable", oldValue, value));
                }
            }
        }

        public Boolean IsExpended
        {
            get { return (Boolean)GetValue(IsExpendedProperty); }
            set
            {
                if (IsExpended != value)
                {
                    SetValue(IsExpendedProperty, value);
                    //OnPropertyChanged(new PropertyChangedEventArgs("IsExpended"));
                }
            }
        }

        // Using a DependencyProperty as the backing store for IsExpended.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsExpendedProperty =
            DependencyProperty.Register("IsExpended", typeof(Boolean), typeof(JFCExpendItem), new UIPropertyMetadata(false, new PropertyChangedCallback(UpdateIsExpendedProperty)));

        private static void UpdateIsExpendedProperty(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            if (e.NewValue != e.OldValue)
            {
                JFCExpendItem item = obj as JFCExpendItem;

                //item.OnPropertyChanged(new PropertyChangedEventArgs("IsExpended"));

                item.OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("IsExpended", e.OldValue, e.NewValue));
            }
        }

        #region INotifyPropertyChanged Members

        //public event PropertyChangedEventHandler PropertyChanged;
        //internal void OnPropertyChanged(PropertyChangedEventArgs e)
        //{
        //    if (PropertyChanged != null)
        //        PropertyChanged(this, e);
        //}

        #endregion

        public event PropertyChangedExtendedEventHandler<Object> PropertyChanged;
        internal void OnPropertyChanged(PropertyChangedExtendedEventArgs<Object> e)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, e);
        }

        public void ResetEvent()
        {
            PropertyChanged = null;
        }
    }
}
