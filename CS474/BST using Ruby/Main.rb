class DoubleSet
  def initialize
    @set = []
  end

  def [](index)
  end

  def list
    puts "Set content: #{@set.join(', ')}"
  end

  def self.list_both(set1, set2)
    puts "Listing both sets:"
    puts "Set 1 content: #{set1.instance_variable_get(:@set).join(', ')}"
    puts "Set 2 content: #{set2.inorder_traversal.join(', ')}" # Assuming set2 is a BSTSet
  end

  def copy(source_set)
    raise NotImplementedError, "This method should be implemented in subclasses"
  end
end

class ArrayedSet < DoubleSet
  def initialize
    @set = []
  end

  def initialize_values(values)
    @set = values.uniq.sort
  end

  def [](index)
    @set[index]
  end

  def list
    super
  end
  def copy(source_set)
    @set = source_set.instance_variable_get(:@set).dup
  end
end

class BSTSet < DoubleSet

  def initialize
    @root = nil
  end

  def initialize_values(values)
    @root = nil
    values.uniq.each do |value|
      @root = insertValuesInBST(@root, value)
      if(@root != nil)

      end
    end
  end

  def insertValuesInBST(node, value)
    return Node.new(value) if node.nil?
    if value < node.value
      node.left = insertValuesInBST(node.left, value)
    else
      node.right = insertValuesInBST(node.right, value)
    end
    return node
  end

  def inorder_traversal(node = @root, result = [])
    return result if node.nil?
    inorder_traversal(node.left, result)
    result << node.value
    inorder_traversal(node.right, result)
    result
  end

  def list
    result = inorder_traversal
    puts "Set content in X: #{result.join(', ')}"
  end

  def copy(source_set)
    @root = nil
    copy_nodes(source_set.instance_variable_get(:@root))
  end

  def copy_nodes(source_node)
    return if source_node.nil?

    insertValuesInBST(@root, source_node.value)
    copy_nodes(source_node.left)
    copy_nodes(source_node.right)
  end

  class Node
    attr_accessor :value, :left, :right

    def initialize(value)
      @value = value
      @left = nil
      @right = nil
    end
  end

end

class Main

  def start
    while true
      puts "Enter your choice: "
      choice = gets.chomp
      case choice
      when "X"
        addBSTElements("4,2,3,5,1")
        list_X
      when "Y"
        addArrayElements("8, 15, 1, 5")
        list_Y
      when "m"
        apply_lambda_to_X("x * 2")
      when "a"
        add_value_to_X(6)
        list_X
      when "u"
        union_x_y
      when "i"
        intersection_X_Y
      when "x"
        puts ""
      when "l"
        DoubleSet.list_both(@Y,@X)
      when "c"
        puts "Which one should copy "
        ans = gets.chomp
        if ans  == "y"
          @Y.copy(@X)
        else
          @X.copy(@Y)
        end
      when "q"
        list_X
        list_Y
        exit
      else
        puts "Invalid choice"
      end
    end
  end

  def initialize
    @Y = ArrayedSet.new
    @X = BSTSet.new
  end

  def addArrayElements(input)
    values = input.split(",").map(&:to_i)
    @Y.initialize_values(values)
    puts "Y initialized with values: #{values.join(', ')}"
  end

  def list_Y
    puts "Set Y:"
    @Y.list
  end

  def addBSTElements(input)
    values = input.split(",").map(&:to_i)
    @X.initialize_values(values)
    puts "X initialized with values: #{values.join(', ')}"
  end

  def apply_lambda_to_X(lambda_string)
    lambda_func = eval("->(x) { #{lambda_string} }")
    elements = @X.inorder_traversal
    transformed_values = elements.map { |value| lambda_func.call(value) }.uniq.sort
    @X.initialize_values(transformed_values)
    puts "Set X after applying lambda: #{transformed_values.join(', ')}"
  end


  def add_value_to_X(value)
    @X.initialize_values(insert_value(@X.inorder_traversal, value))
    puts "#{value} added to Set X."
  end

  def insert_value(current_values, value)
    return current_values if current_values.include?(value) # Skip duplicates
    current_values << value
    current_values.uniq.sort
  end

  def union_x_y
    @Y.instance_variable_get(:@set).each do |value|
      add_value_to_X(value)
    end
    puts "Union applied. Set X now contains: #{ @X.inorder_traversal.join(', ') }"
  end

  def intersection_X_Y
    elements_Y = @Y.instance_variable_get(:@set) # Get elements from Y
    @X.instance_variable_set(:@root, prune_BST(@X.instance_variable_get(:@root), elements_Y))
    puts "After intersection, Set X: #{@X.inorder_traversal.join(', ')}"
  end

  def prune_BST(node, elements_Y)
    return nil if node.nil?
    node.left = prune_BST(node.left, elements_Y)
    node.right = prune_BST(node.right, elements_Y)
    unless elements_Y.include?(node.value)
      if node.left.nil?
        return node.right
      elsif node.right.nil?
        return node.left
      end
      min_node = find_min(node.right)
      node.value = min_node.value
      node.right = remove_min(node.right)
    end

    return node
  end

  def remove_min(node)
    return node.right if node.left.nil?
    node.left = remove_min(node.left)
    node
  end

  def find_min(node)
    current = node
    current = current.left while current.left
    current
  end


  def list_X
    @X.list
  end

  def swap_sets
    y_values = @Y.instance_variable_get(:@set).dup
    @Y.initialize_values(@X.inorder_traversal)
    @X.initialize_values(y_values)
    puts "Sets swapped. New Y: #{ @Y.instance_variable_get(:@set).join(', ') } | New X: #{ @X.inorder_traversal.join(', ') }"
  end

end

calc = Main.new
calc.start

