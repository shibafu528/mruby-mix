module Plugin::World
  module Gate
    unless method_defined?(:accounts)
      # Host Appに使用可能なアカウントのリストを問い合わせる。
      # @return [Array<Diva::Model>]
      def self.accounts
        []
      end
    end
  end
end
