#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  private:
  struct Node
  {
    // Creates a node with the given key
    Node(KeyType key):m_key(key),m_left(nullptr),m_right(nullptr){}
    KeyType m_key;
    std::vector<ValueType> m_values;
    Node* m_left;
    Node* m_right;
  };
  Node* m_root;
  // Deletes all the nodes in the tree
  void clear(Node* node){
    if(node == nullptr){
      return;
    }
    clear(node->m_left);
    clear(node->m_right);
    delete node;
  }
  public:
    class Iterator
    {
      public:
        Iterator()
        {
          m_currNode = nullptr;
          m_index = 0;
        }

        Iterator(Node* node):m_currNode(node),it(node->m_values.begin()){
          m_index = 0;
        }

        ValueType& get_value() const
        {
          return (*it);
        }

        bool is_valid() const
        {
          if(m_currNode == nullptr){
            return false;
          }
          if(m_index >= m_currNode->m_values.size()){
            return false;
          }
          return true;
        }

        void advance()
        {
          it++;
          m_index++;
        }

      private:
        Node* m_currNode;
        typename std::vector<ValueType>::iterator it;
        int m_index;
    };

    TreeMultimap()
    {
      m_root = nullptr;
    }

    ~TreeMultimap()
    {
      clear(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
      // // Check if there is no root node
      if(m_root == nullptr){
        m_root = new Node(key);
        m_root->m_values.push_back(value);
      }
      // // Check if there are existing nodes with the key
      else if(find(key).is_valid() == true){
        Node* temp = m_root;
        while(temp != nullptr){
          if(temp->m_key == key){
            temp->m_values.push_back(value);
            break;
          }
          else if(temp->m_key > key){
            temp = temp->m_left;
          }
          else{
            temp = temp->m_right;
          }
        }
      }
      // // Check if there are no existing nodes with the key
      else{
        Node* temp = m_root;
        while(temp != nullptr){
          if(temp->m_key > key){
            if(temp->m_left == nullptr){
              temp->m_left = new Node(key);
              temp->m_left->m_values.push_back(value);
              break;
            }
            else{
              temp = temp->m_left;
            }
          }
          else{
            if(temp->m_right == nullptr){
              temp->m_right = new Node(key);
              temp->m_right->m_values.push_back(value);
              break;
            }
            else{
              temp = temp->m_right;
            }
          }
        }
      }
    }

    Iterator find(const KeyType& key) const
    {
      Node* temp = m_root;
      while(temp != nullptr){
        if(temp->m_key == key){
          return Iterator(temp);
        }
        else if(temp->m_key > key){
          temp = temp->m_left;
        }
        else{
          temp = temp->m_right;
        }
      }
      return Iterator();
    }
};

#endif // TREEMULTIMAP_INCLUDED
