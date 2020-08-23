# UserConfig compatibility

class UserConfig
  include Singleton
  include ConfigLoader

  class << self

    def include?(key)
      UserConfig.instance.include?(key)
    end

    def [](key)
      UserConfig.instance.at(key)
    end

    def []=(key, value)
      UserConfig.instance.store(key, value)
    end

  end
end
