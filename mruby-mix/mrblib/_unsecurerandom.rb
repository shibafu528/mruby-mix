module UnsecureRandom
  # NOT UUID, but useful for identity
  def self.uuid
    @@ident ||= 0
    @@ident += 1
    _fnv(@@ident.to_s, 32).to_s(16)
  end
end

class Pluggaloid::Plugin
  SecureRandom = ::UnsecureRandom
end

class Pluggaloid::Identity
  SecureRandom = ::UnsecureRandom
end
