class Command
  attr_reader :opcode

  def initialize
    @opcode = ""
  end

  def execute
    raise NotImplementedError, "execute method is not implemented"
  end
end

class ALI
  attr_accessor :memory, :accumulator, :data_register, :program_counter, :zero_bit

  def initialize
    @memory = Array.new(256,["halt",0]) #initialize with halt
    @accumulator = 0
    @data_register = 0
    @program_counter = 0
    @zero_bit = 0
  end

  def load(value)
    @accumulator = @memory[128 +value][1]
  end

  def store(value)
    @memory[128 +value] = ["data", @accumulator]
  end

  def add(value)
    result = @accumulator + @memory[128 + value][1]
    @accumulator = result % 65536
    set_zero_bit(@accumulator)
  end

  def sub(value)
    result = @accumulator -@memory[128 + value][1]
    @accumulator = result % 65536
    set_zero_bit(@accumulator)
  end

  def jump(value)
    @program_counter = value-1
  end

  def jump_if_zero(value)
    @program_counter = value-1 if @zero_bit == 1
  end

  def halt
    puts "HALT reached ending program"
  end

  def set_zero_bit(result)
    @zero_bit = result.zero? ? 1 : 0
  end

  def display_state
    puts "PC: #{@program_counter} | accumulator: #{@accumulator} | zero Bit: #{@zero_bit}"
    puts "memory (Program):"
    @memory.each_with_index do |value, index|
      next if value == ["halt", 0]  #skip if uninitialized
      label = index < 128 ? "program" : "data"
      puts "#{label} memory[#{index}]: #{value}"
    end
  end
end

class LoadCommand < Command
  def initialize(receiver, value)
    super()
    @receiver = receiver
    @value = value
    @opcode = "LOAD"
  end

  def execute
    @receiver.load(@value)
  end
end

class StoreCommand < Command
  def initialize(receiver, value)
    super()
    @receiver = receiver
    @value = value
    @opcode = "STORE"
  end

  def execute
    @receiver.store(@value)
  end
end

class AddInstruction < Command
  def initialize(receiver, value)
    super()
    @receiver = receiver
    @value = value
    @opcode = "add"
  end

  def execute
    @receiver.add(@value)
  end
end

class SubInstruction < Command
  def initialize(receiver, value)
    super()
    @receiver = receiver
    @value = value
    @opcode = "sub"
  end

  def execute
    @receiver.sub(@value)
  end
end

class JumpCommand < Command
  def initialize(receiver,value)
    super()
    @receiver = receiver
    @value = value
    @opcode = "JUMP"
  end

  def execute
    @receiver.jump(@value)
  end
end

class JumpIfZeroCommand < Command
  def initialize(receiver ,value)
    super()
    @receiver = receiver
    @value = value
    @opcode = "JZ"
  end

  def execute
    @receiver.jump_if_zero(@value)
  end
end

class HaltCommand < Command
  def initialize(receiver)
    super()
    @receiver = receiver
    @opcode = "halt"
  end

  def execute
    @receiver.halt
  end
end

class CommandLoop
  def initialize(commands,ali)
    @commands = commands
    @ali = ali
    @program_counter = 0
  end

  def execute_s
    command = @commands[@program_counter]
    command.execute
    @ali.program_counter+=1
    @ali.display_state
  end

  def execute_a
    while @program_counter < @commands.size
      command = @commands[@program_counter]
      command.execute
      break if command.is_a?(HaltCommand)
      @program_counter += 1
    end
    @ali.display_state
  end
end

#client
class Main
  def initialize
    @ali = ALI.new
    commands = load_program
    @command_loop = CommandLoop.new(commands, @ali)
    command_loop
  end

  def load_program
    commands = []
    print "Enternfilename:"
    filename = gets.chomp

    if File.exist?(filename)
      File.readlines(filename).each_with_index do |line, index|
        command, operand = line.strip.split
        operand = operand.to_i if operand
        @ali.memory[index] = [command.upcase, operand]

        case command.upcase
        when "LOAD"
          commands << LoadCommand.new(@ali, operand)
        when "STORE"
          commands << StoreCommand.new(@ali, operand)
        when "ADD"
          commands << AddInstruction.new(@ali, operand)
        when "SUB"
          commands << SubInstruction.new(@ali, operand)
        when "JUMP"
          commands << JumpCommand.new(@ali, operand)
        when "JZ"
          commands << JumpIfZeroCommand.new(@ali, operand)
        when "HALT"
          commands << HaltCommand.new(@ali)
        else
          puts "invalid input"
        end
      end
    else
      puts "file not found"
    end

    commands
  end

  def command_loop
    loop do
      print "\nEnter command:"
      command = gets.chomp.downcase

      case command
      when 's'
        @command_loop.execute_s
      when 'a'
        @command_loop.execute_a
        break
      when 'q'
        puts "Exiting loop"
        break
      else
        puts "Invalid command"
      end
    end
  end
end

Main.new