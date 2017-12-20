using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Collections.Specialized;

namespace JFCGridControl
{
    public class JFCGridColumn : JFCGridColumnBase
    {
        public JFCGridColumn()
        {
            Header = new JFCGridColumnHeader();

            item = new JFCGridItem(null);
            item.Orientation = Orientation.Vertical;

            childrenColumns.Parent = this;
            childrenColumns.CollectionChanged += new System.Collections.Specialized.NotifyCollectionChangedEventHandler(childrenColumns_CollectionChanged);
        }

        public JFCGridColumn(string BindingPath)
        {
            this.BindingPath = BindingPath;
            Header = new JFCGridColumnHeader();

            item = new JFCGridItem(null);
            item.Orientation = Orientation.Vertical;

            childrenColumns.Parent = this;
            childrenColumns.CollectionChanged += new System.Collections.Specialized.NotifyCollectionChangedEventHandler(childrenColumns_CollectionChanged);
        }

        public JFCGridColumn(Func<object, object> getterHandler)
            : base(getterHandler)
        {
            Header = new JFCGridColumnHeader();

            item = new JFCGridItem(null);
            item.Orientation = Orientation.Vertical;

            childrenColumns.Parent = this;
            childrenColumns.CollectionChanged += new System.Collections.Specialized.NotifyCollectionChangedEventHandler(childrenColumns_CollectionChanged);
        }

        public enum FrozenType
        {
            None,
            Start,
            End
        }

        private FrozenType frozen = FrozenType.None;
        public FrozenType Frozen
        {
            get { return frozen; }
            set
            {
                var oldvalue = frozen;
                frozen = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Frozen", oldvalue, value));
            }
        }

        private JFCGridColumnHeader header;
        public JFCGridColumnHeader Header
        {
            get { return header; }
            set
            {
                var oldvalue = header;
                header = value;
                header.Column = this;
                //header.SizeChanged += new SizeChangedEventHandler(header_SizeChanged);

                header.Click += new RoutedEventHandler(header_Click);
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Header", oldvalue, value));
            }
        }

        void header_Click(object sender, RoutedEventArgs e)
        {
            OnHeaderClick(this, e);
        }

        // Méthode qui remet à jour la taille de la colonne
        void header_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            //if (!(e.PreviousSize.Height == 0 && e.PreviousSize.Width == 0))
            //    Width = new GridLength(e.NewSize.Width);
            //ActualWidth = new GridLength(e.NewSize.Width);
        }

        public event RoutedEventHandler HeaderClick;
        internal void OnHeaderClick(object sender, RoutedEventArgs e)
        {
            if (HeaderClick != null)
                HeaderClick(this, e);
        }


        private bool isResizable = true;
        public bool IsResizable
        {
            get { return isResizable; }
            set
            {
                bool newvalue = false;
                if (isResizable != value)
                    newvalue = true;

                var oldvalue = isResizable;
                isResizable = value;

                if (newvalue == true)
                    OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("IsResizable", oldvalue, value));
            }
        }

        private bool isMovable = true;
        public bool IsMovable
        {
            get { return isMovable; }
            set
            {
                bool newvalue = false;
                if (isMovable != value)
                    newvalue = true;

                var oldvalue = isMovable;
                isMovable = value;

                if (newvalue == true)
                    OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("IsMovable", oldvalue, value));
            }
        }

        private bool isSelected = false;
        public bool IsSelected
        {
            get { return isSelected; }
            set
            {
                bool newvalue = false;
                if (isSelected != value)
                    newvalue = true;

                var oldvalue = isSelected;
                isSelected = value;

                item.IsSelected = value;

                if (newvalue == true)
                {
                    OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("IsSelected", oldvalue, value));

                    if (this.ChildrenColumns != null)
                    {
                        foreach (var col in this.ChildrenColumns)
                        {
                            col.IsSelected = isSelected;
                        }
                    }
                }
            }
        }

        private ObservableCollection<JFCGridColumn> childrenColumns = new ObservableCollection<JFCGridColumn>();
        public ObservableCollection<JFCGridColumn> ChildrenColumns
        {
            get { return childrenColumns; }
            //set
            //{
            //    childrenColumns = value;
            //    OnPropertyChanged(new PropertyChangedEventArgs("ChildrenColumns"));
            //}
        }

        void childrenColumns_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (childrenColumns.Count() > 0)
                HaveChildren = true;
            else
                HaveChildren = false;

            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                foreach (JFCGridColumn col in e.NewItems)
                {
                    col.Parent = this;
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Move)
            {
                //foreach (JFCGridColumn col in e.NewItems)
                //{
                //    col.Parent = this;
                //}
            }
            else if (e.Action == NotifyCollectionChangedAction.Remove)
            {
                foreach (JFCGridColumn col in e.OldItems)
                {
                    col.Parent = null;
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Replace)
            {
                foreach (JFCGridColumn col in e.NewItems)
                {
                    col.Parent = this;
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Add)
            {
                foreach (JFCGridColumn col in e.OldItems)
                {
                    col.Parent = null;
                }
            }
        }

        public Boolean HaveChildren
        {
            get { return (Boolean)GetValue(HaveChildrenProperty); }
            set { SetValue(HaveChildrenProperty, value); }
        }

        // Using a DependencyProperty as the backing store for HaveChildren.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HaveChildrenProperty =
            DependencyProperty.Register("HaveChildren", typeof(Boolean), typeof(JFCGridColumn), new UIPropertyMetadata(false));

        public GridLength actualWidthHeader = new GridLength();
        public GridLength ActualWidthHeader
        {
            get { return actualWidthHeader; }
            set
            {
                bool newvalue = false;
                if (actualWidthHeader != value)
                    newvalue = true;

                var oldvalue = actualWidthHeader;
                actualWidthHeader = value;

                if (newvalue == true)
                    OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("ActualWidthHeader", oldvalue, value));

                if (childrenColumns.Count() == 0)
                {
                    if (actualWidth.Value < actualWidthHeader.Value)
                        ActualWidth = ActualWidthHeader;
                }
            }
        }

        public override GridLength ActualWidth
        {
            get
            {
                if (childrenColumns.Count() > 0)
                {
                    double w = 0;

                    foreach (var col in childrenColumns)
                    {
                        w += col.ActualWidth.Value;
                    }

                    return new GridLength(w);
                }

                return actualWidth;
            }
            set
            {
                bool newvalue = false;
                if (actualWidth != value)
                    newvalue = true;

                var oldvalue = actualWidth;
                actualWidth = value;

                if (newvalue == true)
                    OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("ActualWidth", oldvalue, value));
            }
        }

        public override GridLength Width
        {
            get
            {
                if (childrenColumns.Count() > 0)
                {
                    double w = 0;

                    foreach (var col in childrenColumns)
                    {
                        w += col.Width.Value;
                    }

                    return new GridLength(w);
                }

                return width;
            }
            set
            {
                bool newvalue = false;

                if (width != value)
                    newvalue = true;

                var oldvalue = width;
                width = value;

                if (newvalue == true)
                    //OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Width", oldvalue, value));
                    OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Width", oldvalue, value));

                if (!width.IsAuto)
                    ActualWidth = width;
            }
        }

        public enum TypesColumn
        {
            Normal,
            Hierarchical,
            GroupingNoRow,
            GroupingWithRow,
            GroupingWithRowWhenNoChildren
        }

        private TypesColumn typeColumn = TypesColumn.Normal;
        public TypesColumn TypeColumn
        {
            get { return typeColumn; }
            set
            {
                bool newvalue = false;
                if (typeColumn != value)
                    newvalue = true;

                var oldvalue = typeColumn;
                typeColumn = value;

                if (newvalue == true)
                    OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("TypeColumn", oldvalue, value));
            }
        }

        private int levelGrouping = -1;
        public int LevelGrouping
        {
            get { return levelGrouping; }
            set
            {
                var oldvalue = levelGrouping;
                levelGrouping = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("LevelGrouping", oldvalue, value));
            }
        }

        private JFCGridItem item = null;
        public JFCGridItem Item
        {
            get { return item; }
            set
            {
                var oldvalue = item;
                item = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Item", oldvalue, value));
            }
        }
        
        private object tag = null;
        public object Tag
        {
            get { return tag; }
            set
            {
                var oldvalue = tag;
                tag = value;
                OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Tag", oldvalue, value));
            }
        }
    }
}
