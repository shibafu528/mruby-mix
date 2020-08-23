# core/config.rb, core/environment.rb compatibility

module CHIConfig
  NAME = "Embeddable mikutter runtime"
  ACRO = "mruby-mix"

  PIDFILE = "/tmp/mikutter.pid"

  CONFROOT = "/tmp/mikutter"

  TMPDIR = File.join(CONFROOT, 'tmp')

  LOGDIR = File.join(CONFROOT, 'log')

  SETTINGDIR = File.join(CONFROOT, 'settings')

  CACHE = File.join(CONFROOT, 'cache')

  PLUGIN_PATH = File.join(CONFROOT, 'plugin')

  AutoTag = false

  NeverRetrieveOverlappedMumble = false

  REVISION = 9999

  VERSION = [4, 1, 0, REVISION]
end

module Environment
  NAME = CHIConfig::NAME

  ACRO = CHIConfig::ACRO

  PIDFILE = CHIConfig::PIDFILE

  CONFROOT = CHIConfig::CONFROOT

  TMPDIR = CHIConfig::TMPDIR

  LOGDIR = CHIConfig::LOGDIR

  SETTINGDIR = CHIConfig::SETTINGDIR

  CACHE = CHIConfig::CACHE

  PLUGIN_PATH = CHIConfig::PLUGIN_PATH

  AutoTag = CHIConfig::AutoTag

  NeverRetrieveOverlappedMumble = CHIConfig::NeverRetrieveOverlappedMumble

  class Version
    include Comparable

    attr_reader :mejor, :minor, :debug, :devel

    def initialize(mejor, minor, debug, devel=0)
      @mejor = mejor
      @minor = minor
      @debug = debug
      @devel = devel
    end

    def to_a
      [@mejor, @minor, @debug, @devel]
    end

    def to_s
      if 9999 == @devel
        [@mejor, @minor, @debug].join('.')
      else
        [@mejor, @minor, @debug, @devel].join('.')
      end
    end

    def to_i
      @mejor
    end

    def to_f
      @mejor + @minor/100
    end

    def inspect
      "#{Environment::NAME} ver.#{self.to_s}"
    end

    def size
      to_a.size
    end

    def <=>(other)
      self.to_a <=> other.to_a
    end

  end

  VERSION = Version.new(*CHIConfig::VERSION)

end
