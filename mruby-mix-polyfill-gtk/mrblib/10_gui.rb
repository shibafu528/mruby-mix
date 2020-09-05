# encoding: utf-8
# core/plugin/gui 互換プラグイン

module Plugin::GUI
  class Event < Diva::Model
    field.string :event, required: true
    # field.has :widget, Diva::Model, required: true
    field.has :messages, [Diva::Model]
    field.has :world, Diva::Model, required: true

    def widget
      self[:widget]
    end
  end

  # 一時的にPlugin::GUI::Widgetを生成する際に、後始末を確実に実行するためのユーティリティ
  class EphemeralSession < Set
    class << self
      def begin(&block)
        session = EphemeralSession.new
        @stack ||= []
        @stack << session
        block.()
      ensure
        session.destroy
        @stack.pop
      end

      def current
        @stack ||= []
        @stack.last
      end
    end

    def destroy
      each do |obj|
        if obj.respond_to? :destroy
          obj.destroy
        end
        if obj.class.include? InstanceStorage
          obj.class.destroy(obj.name)
        end
      end
    end
  end

  # EphemeralSessionでの自動削除に対応させる
  module Ephemeral
    def initialize(*args)
      super
      if session = EphemeralSession.current
        session << self
      end
    end
  end

  module Widget
    def initialize(*args)
      super
      @destroyed = false
    end

    def destroy
      unless destroyed?
        Plugin.call(:gui_destroy, self)
        @destroyed = true
      end
    end

    def destroyed?
      @destroyed
    end
  end

  # あまりmikutterのPlugin::GUI::Cuscadableとは関係ない、ただのインスタンスストレージ
  module CuscadableLite
    def self.included(klass)
      super
      klass.include(InstanceStorage)
      klass.extend(ClassMethods)
    end

    def slug
      name
    end

    module ClassMethods
      def instance(slug = nil)
        unless slug
          slug = "//#{self.to_s}/#{Time.now.to_i.to_s(16)}/#{Delayer.ticks}".to_sym
        end
        self[slug]
      end

      # deprecated! this is backward compatibility for yukari-exvoice
      def singleton
        instance(:singleton)
      end
    end
  end
end

