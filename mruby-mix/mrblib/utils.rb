# core/utils.rb compatibility

HYDE = 156

def notice(msg)
  log "notice", msg
end

def warn(msg)
  log "warning", msg
end

def error(msg)
  log "error", msg
end

def log(prefix, object)
  begin
    msg = "#{prefix}: #{object}"
    msg += "\nfrom " + object.backtrace.join("\nfrom ") if object.is_a? Exception

    if msg.is_a? Exception
      Mix.log msg.to_s
      Mix.log msg.backtrace.join("\n")
    else
      Mix.log msg
    end
  rescue Exception => e
    Mix.log "critical!: #{e.to_s}"
    Mix.log e.backtrace.join("\n")
  end
end

class Array
  include Comparable

  def symbolize
    result = []
    each { |val|
      result << if val.respond_to?(:symbolize) then val.symbolize else val end }
    result
  end
end

class Hash
  def symbolize
    result = {}
    each_pair { |key, val|
      result[key.to_sym] = if val.respond_to?(:symbolize) then val.symbolize else val end }
    result
  end
end
