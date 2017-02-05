$hello = 0

print <<"foo", <<"bar"  # you can stack them
	I said foo.
foo
	I said bar.
bar


print <<EOF
    This is the first way of creating
    here document ie. multiple line string.
EOF

print <<`EOC`                 # execute commands
	echo hi there
	echo lo there
EOC

puts "This is main Ruby Program"

BEGIN {
   puts "Initializing Ruby Program"
}

class Hi
    @@no_of_customers = 0
    def printing()
	puts "Hi"
    end	
end

hi = Hi.new
hi.printing()

class Customer
   @@no_of_customers=0
   def initialize(id, name, addr)
      @cust_id=id
      @cust_name=name
      @cust_addr=addr
   end
   def display_details()
      puts "Customer id #@cust_id"
      puts "Customer name #@cust_name"
      puts "Customer address #@cust_addr"
   end
   def total_no_of_customers()
      @@no_of_customers += 1
      puts "Total number of customers: #@@no_of_customers"
   end
end

# Create Objects
cust1=Customer.new("1", "John", "Wisdom Apartments, Ludhiya")
cust2=Customer.new("2", "Poul", "New Empire road, Khandala")

# Call Methods
cust1.display_details()
cust1.total_no_of_customers()
cust2.display_details()
cust2.total_no_of_customers()
cust1.total_no_of_customers()

(10..15).each do |n| 
   print n, ' ' 
end

puts " "

(1...10).each do |n|
   print n, ' '
end

$global = defined? $hello
puts "#$global"

class Rectangle
def initialize(width,length) 
@width = width
@length = length
end
def getWidth()
return @width
end
def printWidth()
puts @width
end
end

hi = Rectangle.new(10,10)
hi.printWidth()

