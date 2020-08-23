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
      puts msg.to_s
      puts msg.backtrace.join("\n")
    else
      puts msg
    end
  rescue Exception => e
    puts "critical!: #{e.to_s}"
    puts e.backtrace.join("\n")
  end
end
