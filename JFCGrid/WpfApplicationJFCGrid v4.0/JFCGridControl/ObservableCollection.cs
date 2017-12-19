// JFCGridControl.ObservableCollection<T>
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;


namespace JFCGridControl
{
    public class ObservableCollection<T> : System.Collections.ObjectModel.ObservableCollection<T>
    {
        private object parent;

        public object Parent
        {
            get
            {
                return this.parent;
            }
            set
            {
                this.parent = value;
                this.OnPropertyChanged(new PropertyChangedEventArgs("Parent"));
            }
        }

        public void AddRange(IEnumerable<T> collection)
        {
            int count = base.Count;
            List<T> list = new List<T>();
            list.AddRange(collection);
            foreach (T item in collection)
            {
                base.Items.Add(item);
            }
            NotifyCollectionChangedEventArgs e = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, list);
            this.OnCollectionChanged(e);
        }

        public void RemoveRange(IEnumerable<T> collection)
        {
            List<T> list = new List<T>();
            list.AddRange(collection);
            foreach (T item in collection)
            {
                base.Items.Remove(item);
            }
            NotifyCollectionChangedEventArgs e = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, list);
            this.OnCollectionChanged(e);
        }

        public void Replace(T item)
        {
            this.ReplaceRange(new T[1]
            {
            item
            });
        }

        public void ReplaceRange(IEnumerable<T> collection)
        {
            new List<T>(base.Items);
            base.Items.Clear();
            foreach (T item in collection)
            {
                base.Items.Add(item);
            }
            this.OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public ObservableCollection()
        {
        }

        public ObservableCollection(IEnumerable<T> collection)
            : base(collection)
        {
        }

        public ObservableCollection(object Parent)
        {
            this.Parent = Parent;
        }

        public ObservableCollection(object Parent, IEnumerable<T> collection)
            : base(collection)
        {
            this.Parent = Parent;
        }
    }
}