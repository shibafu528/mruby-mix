class String
  def ascii_only?
    !(self =~ /[^\x00-\x7f]/)
  end unless method_defined? :ascii_only?
end
