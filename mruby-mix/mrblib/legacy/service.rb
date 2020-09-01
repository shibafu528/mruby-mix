# Service compatibility for mikutter 3.5 or before plugins

module Service
  extend Enumerable

  UnsupportedOperationError = Class.new(StandardError)

  class << self
    def instances
      collect(:worlds)
    end
    alias services instances

    def each(*args, &proc)
      instances.each(*args, &proc)
    end

    def primary
      world, = Plugin.filtering(:world_current, nil)
      world
    end
    alias primary_service primary

    def primary!
      result = primary
      raise Plugin::World::NotExistError, 'World does not exists.' unless result
      result
    end

    def set_primary(service)
      raise UnsupportedOperationError
    end

    def destroy(service)
      raise UnsupportedOperationError
    end
    alias remove_service destroy
  end
end

Post = Service
