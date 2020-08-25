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
